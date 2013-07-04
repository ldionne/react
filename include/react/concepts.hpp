/*!
 * @file
 * This file defines the concepts used in the library.
 */

#ifndef REACT_CONCEPTS_HPP
#define REACT_CONCEPTS_HPP

#include <react/archetypes.hpp>
#include <react/detail/call_computation.hpp>
#include <react/detail/pointers_to.hpp>
#include <react/traits.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>


namespace react {
/*!
 * Specification of the `Feature` concept.
 *
 * A `Feature` represents an abstract functionality to be provided by
 * a `FeatureSet`. The `Feature` is completely independent from its
 * implementation, which is handled by an `IncrementalComputation`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `F`        | A type modeling the `Feature` concept
 * | `Args...`  | An arbitrary sequence of types
 *
 *
 * ## Valid expressions
 * | Expression                     | Return type                 | Semantics
 * | ----------                     | -----------                 | ---------
 * | `apply<F, Args...>::type`      | An `IncrementalComputation` | The implementation of the feature when parameterized with `Args...`.
 * | `apply<F>::type`<sub>opt</sub> | An `IncrementalComputation` | The default implementation of the feature. The expression is only required if the feature is required in a feature set but its implementation is not explicitly specified.
 *
 *
 * @tparam F
 *         The type to be tested for modeling of the `Feature` concept.
 */
template <typename F, typename ...Args>
struct Feature {
    BOOST_CONCEPT_USAGE(Feature) {
        using Implementation = typename boost::mpl::apply<F, Args...>::type;
    }
};

/*!
 * Specification of the `CompositeFeature` concept.
 *
 * A `CompositeFeature` is a bundle of several related features together.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `CF`       | A type modeling the `CompositeFeature` concept
 *
 *
 * ## Valid expressions
 * | Expression                | Return type                                     | Semantics
 * | ----------                | -----------                                     | ---------
 * | `subfeatures_of<F>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s | The set of features bundled along with `F`. See `subfeatures_of` for details.
 *
 *
 * @tparam CF
 *         The type to be tested for modeling of the `CompositeFeature`
 *         concept.
 */
template <typename CF>
struct CompositeFeature {
    BOOST_CONCEPT_USAGE(CompositeFeature) {
        using Subfeatures = typename subfeatures_of<CF>::type;
        static_assert(boost::mpl::is_sequence<Subfeatures>::value,
        "the `subfeatures_of<CF>::type` expression should be an MPL "
        "sequence of types modeling the `Feature` concept");
    }
};


struct default_construct;

namespace concept_detail {
    template <typename T> T get();

    template <typename T, typename Args>
    struct construct {
        void operator()() const {
            Args& args = get<Args&>();
            T x{args}; (void)x;
        }
    };

    template <typename T>
    struct construct<T, default_construct> {
        void operator()() const {
            T x{}; (void)x;
        }
    };
}

/*!
 * Specification of the `IncrementalComputation` concept.
 *
 * An `IncrementalComputation` is a concrete implementation of the
 * functionality represented by a `Feature`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `IC`       | A type modeling the `IncrementalComputation` concept
 * | `ic`       | An object of type `IC`
 * | `fs`       | An arbitrary `FeatureSet`
 * | `args`     | An arbitrary Boost.Parameter `ArgumentPack`
 * | `tag`      | An object convertible to any other object of type `T` for which either `apply<T, X>` or `matches<T, X>` returns true, for some type `X`
 *
 *
 * ## Valid expressions
 * | Expression                  | Return type                                     | Semantics
 * | ----------                  | -----------                                     | ---------
 * | `IC x{args}` or `IC x{}`    | None                                            | Construct the computation object, providing the arguments contained in `args`. If `IC` is not constructible with `args`, it must be default constructible.
 * | `ic(tag, fs)`<sub>opt</sub> | Any type                                        | Execute the computation with the semantic tag `tag` and the features provided by the feature set `fs`. If the expression is invalid, the computation is said to be non-executable for this combination of arguments.
 * | `ic.result(fs)`             | Any type                                        | Return the current result computed by the computation object.
 * | `dependencies_of<IC>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s | The set of features required in a feature set in order for this computation to be able to execute with any semantic tag. See `dependencies_of` for details.
 * | `feature_of<IC>::type`      | A `Feature`                                     | The feature implemented by `IC`. See `feature_of` for details.
 *
 *
 * @tparam IC
 *         The type to be tested for modeling of the `IncrementalComputation`
 *         concept.
 *
 * @tparam ConstructionArgs
 *         An `ArgumentPack` sufficient for constructing `IC`, or
 *         `default_construct` if it should be default-constructed.
 *
 * @tparam SemanticTags
 *         A Boost.MPL `ForwardSequence` of semantic tags with which it is
 *         valid to execute `IC`, given a feature set that implements all
 *         of the dependencies of `IC`.
 *
 * @tparam DependenciesResults
 *         A Boost.MPL `AssociativeSequence` mapping each dependency of `IC`
 *         to its result type.
 */
template <typename IC, typename ConstructionArgs,
          typename SemanticTags, typename DependenciesResults>
struct IncrementalComputation {
    BOOST_CONCEPT_USAGE(IncrementalComputation) {
        concept_detail::construct<IC, ConstructionArgs>{}();

        boost::mpl::for_each<
            typename detail::pointers_to<SemanticTags>::type
        >(call_computation{});

        ic.result(fs);

        using Dependencies = typename dependencies_of<IC>::type;
        static_assert(boost::mpl::is_sequence<Dependencies>::value,
        "the `dependencies_of<IC>::type` expression should be an MPL "
        "sequence of types modeling the `Feature` concept");

        BOOST_CONCEPT_ASSERT((Feature<typename feature_of<IC>::type>));
    }

private:
    struct good_enough_feature_set : feature_set_archetype<> {
        template <typename Feature>
        typename boost::mpl::at<DependenciesResults, Feature>::type
        operator[](Feature const&);
    };

    static good_enough_feature_set& fs;
    static IC& ic;

    struct call_computation {
        template <typename SemanticTag>
        void operator()(SemanticTag*) const {
            detail::call_computation<SemanticTag>(ic, fs);
        }
    };
};

/*!
 * Specification of the `FeatureSet` concept.
 *
 * A `FeatureSet` regroups several features together. It handles the
 * association of incremental computations to features, the resolution
 * of dependencies between features and the execution of computations.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `FS`       | A type modeling the `FeatureSet` concept
 * | `fs, ext`  | Arbitrary `FeatureSet`s
 * | `feature`  | An arbitrary `Feature`
 * | `args`     | An arbitrary Boost.Parameter `ArgumentPack`
 * | `tag`      | An arbitrary object
 *
 *
 * ## Valid expressions
 * | Expression                  | Return type                                                    | Semantics
 * | ----------                  | -----------                                                    | ---------
 * | `FS x{args}` or `FS x{}`    | None                                                           | Create a feature set, constructing all of its computations with an `ArgumentPack` containing a non-strict superset of the arguments in `args`. If a computation can't be constructed with the `ArgumentPack`, it is default-constructed instead. A `FeatureSet` may also (or solely) be default-constructible.
 * | `fs[feature]`               | Any type                                                       | Return the current result of the computation implementing `feature` in the set. If there is no such feature in the set, the expression shall be ill-formed.
 * | `fs(tag)`                   | Any type                                                       | Let `X` be the set of the computations of all the features in the `FeatureSet`, including those only present as a dependency. All of the computations `C` in `X` that are executable with `tag` as a semantic tag and `fs` as a feature set shall be executed in an order such that the computations implementing all of the dependencies of `C` are executed before `C`. Computations that are not executable for that combination of arguments shall not be executed.
 * | `fs(tag, ext)`              | Any type                                                       | Same as `fs(tag)`, except that computations are executed with a feature set containing the union of `fs` and `ext`'s features.
 * | `computations_of<FS>::type` | A Boost.MPL `AssociativeSequence` of `IncrementalComputation`s | The set of all the computations in the feature set.
 *
 *
 * @tparam FS
 *         The type to be tested for modeling of the `FeatureSet` concept.
 *
 * @tparam ConstructionArgs
 *         An `ArgumentPack` sufficient for constructing `FS`, or
 *         `default_construct` if it should be default-constructed.
 *
 * @tparam SemanticTags
 *         A Boost.MPL `ForwardSequence` of semantic tags with which it is
 *         valid to call `FS`.
 *
 * @tparam AccessibleFeatures
 *         A Boost.MPL `ForwardSequence` of `Feature`s whose result may be
 *         extracted from `FS`.
 */
template <typename FS, typename ConstructionArgs,
          typename SemanticTags, typename AccessibleFeatures>
struct FeatureSet {
    BOOST_CONCEPT_USAGE(FeatureSet) {
        concept_detail::construct<FS, ConstructionArgs>{}();

        boost::mpl::for_each<
            typename detail::pointers_to<SemanticTags>::type
        >(call{});

        boost::mpl::for_each<
            typename detail::pointers_to<AccessibleFeatures>::type
        >(retrieve_result{});

        using Computations = typename computations_of<FS>::type;
        static_assert(boost::mpl::is_sequence<Computations>::value,
        "computations_of<FS>::type shall be the set of all the "
        "computations in the feature set");
    }

private:
    static FS& fs;
    static feature_set_archetype<>& fs_archetype;

    struct call {
        template <typename SemanticTag>
        void operator()(SemanticTag* tag) const {
            fs(*tag);
            fs(*tag, fs_archetype);
        }
    };

    struct retrieve_result {
        template <typename Feature>
        void operator()(Feature* feature) const {
            fs[*feature];
        }
    };
};

template <typename ...Pairs>
using dependency_results = boost::mpl::map<Pairs...>;

template <typename ...Tags>
using semantic_tags = boost::mpl::vector<Tags...>;

template <typename ...Features>
using accessible_features = boost::mpl::vector<Features...>;
} // end namespace react

#endif // !REACT_CONCEPTS_HPP
