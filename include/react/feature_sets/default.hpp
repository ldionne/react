/*!
 * @file
 * This file defines `react::feature_sets::default_`.
 */

#ifndef REACT_FEATURE_SETS_DEFAULT_HPP
#define REACT_FEATURE_SETS_DEFAULT_HPP

#include <react/archetypes.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/call_computation.hpp>
#include <react/detail/dont_care.hpp>
#include <react/detail/feature_dependency_graph.hpp>
#include <react/feature_sets/by_ref.hpp>
#include <react/feature_sets/cloaked.hpp>
#include <react/feature_sets/empty.hpp>
#include <react/feature_sets/union.hpp>
#include <react/traits.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/topological_sort.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>


namespace react { namespace feature_sets {
namespace default_detail {
namespace mpl = boost::mpl;

template <typename Computation, typename FeatureSet>
cloaked<by_ref<FeatureSet>, Computation> cloak_for(FeatureSet& fset) {
    return {fset};
}

template <typename Derived>
struct storage_node_base {
protected:
    Derived& all_features() { return static_cast<Derived&>(*this); }

public:
    storage_node_base() = default;

    template <typename FeatureSet>
    explicit storage_node_base(FeatureSet&&) { }

    template <bool always_false = false>
    void operator[](detail::dont_care) const {
        static_assert(always_false,
            "could not find the requested feature in the feature set");
    }
};

template <typename Computation, typename Dependencies>
struct storage_node : Dependencies {
private:
    Computation computation_;

public:
    storage_node() = default;

    template <typename FeatureSet>
    explicit storage_node(FeatureSet& ext, typename boost::enable_if<
        std::is_constructible<Computation, FeatureSet&>
    >::type* = 0)
        : Dependencies{ext},
          computation_{
            make_union(
                make_by_ref(static_cast<Dependencies&>(*this)),
                make_by_ref(ext)
            )
          }
    { }

    template <typename FeatureSet>
    explicit storage_node(FeatureSet& ext, typename boost::disable_if<
        std::is_constructible<Computation, FeatureSet&>
    >::type* = 0)
        : Dependencies{ext}, computation_{}
    { }

    template <typename SemanticTag, typename FeatureSet>
    void operator()(SemanticTag const& tag, FeatureSet& ext) {
        Dependencies::operator()(tag, ext);
        detail::call_computation<SemanticTag>(
            computation_,
            cloak_for<Computation>(
                make_union(
                    make_by_ref(this->all_features()),
                    make_by_ref(ext)
                )
            )
        );
    }

    template <typename SemanticTag>
    void operator()(SemanticTag const& tag) {
        return operator()(tag, empty{});
    }

    using Dependencies::operator[];
    auto operator[](typename feature_of<Computation>::type const&)
    REACT_AUTO_RETURN(
        computation_.result(cloak_for<Computation>(this->all_features()))
    )
};

template <typename Vertex>
struct computation_from_vertex {
    using type = typename Vertex::computation;
};

template <typename ...Computations>
struct make_default {
    using PredefinedComputations = mpl::map<
        mpl::pair<typename feature_of<Computations>::type, Computations>...
    >;

    template <typename Feature>
    struct computation_of
        : mpl::eval_if<
            mpl::has_key<PredefinedComputations, Feature>,
            mpl::at<PredefinedComputations, Feature>,
            mpl::apply<Feature>
        >
    { };

    using DependencyGraph = detail::feature_dependency_graph<
        computation_of<mpl::_1>, mpl::vector<Computations...>
    >;

    using ComputationsInVisitationOrder = typename mpl::transform<
        typename mpl::topological_sort<DependencyGraph>::type,
        computation_from_vertex<mpl::_1>
    >::type;

    struct type;

    using Storage = mpl::inherit_linearly<
        typename mpl::reverse<ComputationsInVisitationOrder>::type,
        storage_node<mpl::_2, mpl::_1>,
        storage_node_base<type>
    >;

    struct type : Storage::type {
        using Storage::type::type;
        using Storage::type::operator=;
    };
};

template <typename ...Computations>
struct pseudo_root_computation : incremental_computation_archetype<> {
    using dependencies = mpl::set<typename feature_of<Computations>::type...>;
};
} // end namespace default_detail

/*!
 * Default implementation of the `FeatureSet` concept.
 *
 * @note
 * On construction, if a computation can't be constructed with some
 * `FeatureSet`, it is default-constructed instead. The `default_` feature
 * set may also be default-constructed, in which case all of its computations
 * are default-constructed.
 */
template <typename ...Computations>
using default_ = cloaked<
    typename default_detail::make_default<Computations...>::type,
    default_detail::pseudo_root_computation<Computations...>
>;
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_DEFAULT_HPP
