/*!
 * @file
 * Defines `react::feature_map`.
 */

#ifndef REACT_FEATURE_MAP_HPP
#define REACT_FEATURE_MAP_HPP

#include <react/sandbox/bind.hpp>
#include <react/sandbox/default_of.hpp>
#include <react/sandbox/feature_of.hpp>
#include <react/sandbox/requirements_of.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/key_type.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/value_type.hpp>
#include <boost/mpl/value_view.hpp>
#include <type_traits>


namespace react {
namespace feature_map_detail {
    namespace mpl = boost::mpl;

    /*!
     * @internal
     * Returns whether a `Requirement` has a default implementation.
     */
    template <typename Requirement>
    constexpr auto has_default(int) -> decltype(
        (typename default_of<Requirement>::type*)nullptr, true
    ) { return true; }

    template <typename Requirement>
    constexpr bool has_default(...) { return false; }


    template <typename Requirements, typename PartialFeatureMap>
    struct make_iterator;

    struct end_iterator;
    struct tag;

    template <typename Requirements>
    struct feature_map_impl {
        using tag = feature_map_detail::tag;

    private:
        friend mpl::begin_impl<tag>;
        using requirements = typename mpl::set_insert_range<
            mpl::set0<>, Requirements
        >::type;
    };

    template <typename Pointee, typename LazyNext>
    struct lazy_next_iterator {
        using type = Pointee;
    };

    template <typename Requirements, typename PartialFeatureMap>
    struct make_non_empty_iterator {

        template <typename First, typename Rest>
        struct make_iterator_with_default {

            template <typename Feature, typename Computation>
            struct instantiate_unseen_feature {
                using NewRequirements = typename mpl::set_insert_range<
                    Rest, typename requirements_of<Computation>::type
                >::type;

                using NewFeatureMap = typename mpl::insert<
                    PartialFeatureMap, mpl::pair<Feature, Computation>
                >::type;

                using type = lazy_next_iterator<
                    Computation, make_iterator<NewRequirements, NewFeatureMap>
                >;
            };

            template <typename Feature, typename Computation>
            struct skip_seen_feature {
                static_assert(std::is_same<
                    typename mpl::at<PartialFeatureMap, Feature>::type,
                    Computation
                >::value,
                "while instantiating the feature_map: two requirements or "
                "more specify different default implementations");

                using type = typename make_iterator<
                    Rest, PartialFeatureMap
                >::type;
            };

            using AvailableComputations = mpl::joint_view<
                // This contains all the fully-bound computations.
                mpl::value_view<PartialFeatureMap>,

                // This contains all the computations that are not bound
                // yet, except the one associated to the current
                // requirement (because we're trying to satisfy it).
                mpl::iterator_range<
                    typename make_iterator<Rest, PartialFeatureMap>::type,
                    end_iterator
                >
            >;

            using Computation = typename bind<
                typename default_of<First>::type,
                AvailableComputations
            >::type;

            using Feature = typename feature_of<Computation>::type;

            using type = typename mpl::if_<
                mpl::has_key<PartialFeatureMap, Feature>,
                skip_seen_feature<Feature, Computation>,
                instantiate_unseen_feature<Feature, Computation>
            >::type::type;
        };

        using First = typename mpl::front<Requirements>::type;
        using Rest = typename mpl::erase_key<Requirements, First>::type;

        using type = typename mpl::if_c<has_default<First>(int{}),
            make_iterator_with_default<First, Rest>,
            make_iterator<Rest, PartialFeatureMap>
        >::type::type;
    };

    template <typename Requirements, typename PartialFeatureMap>
    struct make_iterator
        : mpl::if_<
            mpl::empty<Requirements>,
            mpl::identity<end_iterator>,
            make_non_empty_iterator<Requirements, PartialFeatureMap>
        >::type
    { };
} // end namespace feature_map_detail

/*!
 *
 */
template <typename ...Requirements>
struct feature_map
    : feature_map<typename boost::mpl::set<Requirements...>::type>
{ };

/*!
 *
 */
template <typename Requirements>
struct feature_map<Requirements>
    : boost::mpl::if_<boost::mpl::is_sequence<Requirements>,
        feature_map_detail::feature_map_impl<Requirements>,
        feature_map_detail::feature_map_impl<boost::mpl::set1<Requirements>>
    >::type
{ };
} // end namespace react

namespace boost { namespace mpl {
    template <typename Pointee, typename LazyNext>
    struct next<
        react::feature_map_detail::lazy_next_iterator<Pointee, LazyNext>
    >
        : LazyNext
    { };

    template <>
    struct begin_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap>
        struct apply
            : react::feature_map_detail::make_iterator<
                typename FeatureMap::requirements, map0<>
            >
        { };
    };

    template <>
    struct end_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap>
        struct apply
            : identity<react::feature_map_detail::end_iterator>
        { };
    };

    template <>
    struct at_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap, typename Feature>
        struct apply
            : deref<
                typename find_if<
                    FeatureMap,
                    std::is_same<Feature, key_type<FeatureMap, _1>>
                >::type
            >
        { };
    };

    template <>
    struct has_key_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap, typename Feature>
        struct apply
            : not_<
                std::is_same<
                    typename find_if<
                        FeatureMap,
                        std::is_same<Feature, key_type<FeatureMap, _1>>
                    >::type,
                    typename end<FeatureMap>::type
                >
            >
        { };
    };

    template <>
    struct value_type_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap, typename Computation>
        struct apply
            : identity<Computation>
        { };
    };

    template <>
    struct key_type_impl<react::feature_map_detail::tag> {
        template <typename FeatureMap, typename Computation>
        struct apply
            : react::feature_of<Computation>
        { };
    };
}} // end namespace boost::mpl

#endif // !REACT_FEATURE_MAP_HPP
