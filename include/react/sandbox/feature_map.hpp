/*!
 * @file
 * Defines `react::feature_map`.
 */

#ifndef REACT_FEATURE_MAP_HPP
#define REACT_FEATURE_MAP_HPP

#include <react/sandbox/bind.hpp>
#include <react/sandbox/can_be_bound.hpp>
#include <react/sandbox/default_of.hpp>
#include <react/sandbox/detail/has_default.hpp>
#include <react/sandbox/feature_of.hpp>
#include <react/sandbox/requirements_of.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/partition.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <type_traits>


namespace react {
namespace feature_map_detail {
    namespace mpl = boost::mpl;

    template <typename Dependent, typename T>
    struct make_dependent_on { using type = T; };

    template <
        typename Satisfied = mpl::map0<>,
        typename RequirementsWithoutDefault = mpl::set0<>
    >
    struct in_context {
        /*!
         * @internal
         * See `instantiate_requirement` for why we remove the feature of
         * `Requirement` from the `Satisfied` features before using
         * `can_be_bound`.
         */
        template <typename Requirement>
        struct requirement_can_be_satisfied
            : can_be_bound<
                typename default_of<Requirement>::type,
                typename mpl::erase_key<Satisfied,
                    typename feature_of<Requirement>::type
                >::type
            >
        { };

        template <typename Unsatisfied>
        struct remove_non_defaults;

        template <typename Unsatisfied>
        struct apply
            : remove_non_defaults<Unsatisfied>
        { };

        template <typename Unsatisfied,
                  bool = mpl::empty<Unsatisfied>::value>
        struct handle_all_satisfied;

        template <typename Unsatisfied,
                  typename IterToSatisfy = typename mpl::find_if<
                    Unsatisfied, requirement_can_be_satisfied<mpl::_1>
                  >::type,
                  bool = std::is_same<
                    IterToSatisfy, typename mpl::end<Unsatisfied>::type
                >::value>
        struct check_for_cycle;

        template <typename Unsatisfied, typename ToSatisfy>
        struct instantiate_requirement;

        template <typename Unsatisfied, typename Computation,
                  typename Feature = typename feature_of<Computation>::type,
                  bool = mpl::has_key<Satisfied, Feature>::value>
        struct handle_duplicates;

        /*!
         * @internal
         * First, we filter all requirements without a default implementation.
         * For these requirements, we have nothing to instantiate.
         *
         * We also keep track of the requirements without a default
         * implementation in order to make sure that each of their
         * features are implemented by some computation at some point.
         */
        template <typename Unsatisfied>
        struct remove_non_defaults {
            using Partitioned = typename mpl::partition<
                Unsatisfied,
                detail::has_default<mpl::_1>,
                mpl::inserter<mpl::set0<>, mpl::insert<mpl::_1, mpl::_2>>,
                mpl::inserter<
                    RequirementsWithoutDefault,
                    // We only insert the requirements whose feature was not
                    // already satisfied. This is only an optimization.
                    mpl::if_<
                        mpl::has_key<Satisfied, feature_of<mpl::_2>>,
                        mpl::_1,
                        mpl::insert<mpl::_1, mpl::_2>
                    >
                >
            >::type;

            using type = typename in_context<
                Satisfied, typename mpl::second<Partitioned>::type
            >::template handle_all_satisfied<
                typename mpl::first<Partitioned>::type
            >::type;
        };

        /*!
         * @internal
         * If all the requirements have been satisfied, we're done.
         *
         * We also make sure that the feature of all the requirements that
         * did not provide a default implementation have been implemented.
         */
        template <typename EmptyUnsatisfied>
        struct handle_all_satisfied<EmptyUnsatisfied, true> {
            using UnimplementedFeatures = typename make_dependent_on<
                EmptyUnsatisfied,
                mpl::fold<
                    RequirementsWithoutDefault, mpl::set0<>,
                    mpl::if_<mpl::has_key<Satisfied, feature_of<mpl::_2>>,
                        mpl::_1,
                        mpl::insert<mpl::_1, feature_of<mpl::_2>>
                    >
                >
            >::type::type;

            static_assert(mpl::empty<UnimplementedFeatures>::value,
            "some feature is missing an implementation because no "
            "requirement specified a default implementation for it");

            using type = Satisfied;
        };

        /*!
         * @internal
         * If some requirements are still unsatisfied, we proceed to check
         * for circular dependencies in the substitution.
         */
        template <typename Unsatisfied>
        struct handle_all_satisfied<Unsatisfied, false>
            : check_for_cycle<Unsatisfied>
        { };

        /*!
         * @internal
         * If there is a circular dependency in the substitution required by
         * some requirements, we trigger an assertion and stop here.
         */
        template <typename Unsatisfied, typename IterToSatisfy>
        struct check_for_cycle<Unsatisfied, IterToSatisfy, true> {
            static_assert(
                !make_dependent_on<Unsatisfied, mpl::true_>::type::value,
            "there is a circular dependency in the substitution required "
            "by some requirements");
        };

        /*!
         * @internal
         * If there is no circular dependency in the substitution, we find
         * the next requirement to satisfy and we proceed to instantiate it.
         */
        template <typename Unsatisfied, typename IterToSatisfy>
        struct check_for_cycle<Unsatisfied, IterToSatisfy, false>
            : instantiate_requirement<
                typename mpl::erase_key<Unsatisfied,
                    typename mpl::deref<IterToSatisfy>::type
                >::type,
                typename mpl::deref<IterToSatisfy>::type
            >
        { };

        /*!
         * @internal
         * We instantiate the requirement that we want to satisfy and
         * proceed to check whether we have already instantiated a
         * requirement for the same feature.
         *
         * @note
         * We make sure that `Satisfied` does not contain the feature of
         * the requirement that we're instantiating. Otherwise, if duplicate
         * default implementations were provided, a computation could use
         * `computation_of<self>` to refer to the first default
         * implementation, which is not desirable.
         */
        template <typename Unsatisfied, typename ToSatisfy>
        struct instantiate_requirement
            : handle_duplicates<
                Unsatisfied,
                typename bind<
                    typename default_of<ToSatisfy>::type,
                    typename mpl::erase_key<
                        Satisfied, typename feature_of<ToSatisfy>::type
                    >::type
                >::type
            >
        { };

        /*!
         * @internal
         * If we have not already satisfied a requirement for the same
         * feature, we augment the context with the newly satisfied
         * requirement and we continue in that context.
         */
        template <typename Unsatisfied, typename Computation, typename Feature>
        struct handle_duplicates<Unsatisfied, Computation, Feature, false>
            : in_context<
                typename mpl::insert<
                    Satisfied, mpl::pair<Feature, Computation>
                >::type,
                RequirementsWithoutDefault
            >::template apply<
                typename mpl::set_insert_range<
                    Unsatisfied,
                    typename requirements_of<Computation>::type
                >::type
            >
        { };

        /*!
         * @internal
         * If we have already satisfied a requirement for the same feature,
         * we make sure both default implementations were the same. Otherwise
         * it is ambiguous which implementation should be used.
         *
         * If no ambiguity arises, we just skip this requirement since we
         * have already seen it.
         */
        template <typename Unsatisfied, typename Computation, typename Feature>
        struct handle_duplicates<Unsatisfied, Computation, Feature, true>
            : apply<Unsatisfied>
        {
            static_assert(std::is_same<
                typename mpl::at<Satisfied, Feature>::type,
                Computation
            >::value,
            "two requirements or more specify different default "
            "implementations for the same feature");
        };
    };
} // end namespace feature_map_detail

/*!
 *
 */
template <typename ...Requirements>
struct feature_map
    : feature_map_detail::in_context<>::template
        apply<
            typename boost::mpl::set<Requirements...>::type
        >::type
{ };
} // end namespace react

#endif // !REACT_FEATURE_MAP_HPP
