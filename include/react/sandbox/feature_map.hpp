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
#include <boost/mpl/bool.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <type_traits>


namespace react {
namespace feature_map_detail {
    namespace mpl = boost::mpl;


    template <typename Requirement>
    constexpr auto has_default(int) -> decltype(
        (typename default_of<Requirement>::type*)nullptr, true
    ) { return true; }

    template <typename Requirement>
    constexpr bool has_default(...) { return false; }


    template <typename Requirements, typename PartialFeatureMap>
    struct instantiate_requirements;

    template <typename Requirements, typename PartialFeatureMap>
    struct instantiate_requirements_impl {

        template <typename First, typename Rest>
        struct instantiate_with_default {

            template <typename Feature, typename Computation>
            struct instantiate_unseen {
                using NewRequirements = typename mpl::set_insert_range<
                    Rest, typename requirements_of<Computation>::type
                >::type;

                using NewFeatureMap = typename mpl::insert<
                    PartialFeatureMap, mpl::pair<Feature, Computation>
                >::type;

                using type = typename instantiate_requirements<
                    NewRequirements, NewFeatureMap
                >::type;
            };

            template <typename Feature, typename Computation>
            struct skip_seen {
                static_assert(std::is_same<
                    typename mpl::at<PartialFeatureMap, Feature>::type,
                    Computation
                >::value,
                "while instantiating the feature_map: two requirements or "
                "more specify different default implementations");

                using type = typename instantiate_requirements<
                    Rest, PartialFeatureMap
                >::type;
            };


            using Computation = typename bind<
                typename default_of<First>::type,
                instantiate_requirements<Rest, PartialFeatureMap>
            >::type;

            using Feature = typename feature_of<Computation>::type;

            using type = typename mpl::if_<
                mpl::has_key<PartialFeatureMap, Feature>,
                skip_seen<Feature, Computation>,
                instantiate_unseen<Feature, Computation>
            >::type::type;
        };

        using First = typename mpl::front<Requirements>::type;
        using Rest = typename mpl::erase_key<Requirements, First>::type;

        using type = typename mpl::if_c<has_default<First>(int{}),
            instantiate_with_default<First, Rest>,
            instantiate_requirements<Rest, PartialFeatureMap>
        >::type::type;
    };

    template <typename Requirements, typename PartialFeatureMap>
    struct instantiate_requirements
        : mpl::if_<
            mpl::empty<Requirements>,
            mpl::identity<PartialFeatureMap>,
            instantiate_requirements_impl<Requirements, PartialFeatureMap>
        >::type
    { };
} // end namespace feature_map_detail

/*!
 *
 */
template <typename ...Requirements>
struct feature_map
    : feature_map_detail::instantiate_requirements<
        typename boost::mpl::set<Requirements...>::type,
        boost::mpl::map0<>
    >::type
{ };
} // end namespace react

// Several semi-failed attempts to make a lazy feature map. Maybe a lazy
// feature map is not what we need after all.
#if 0
    template <typename ToVisit,
              typename Visited,
              typename Requirement = typename mpl::front<ToVisit>::type,
              bool has_default = has_default<Requirement>::value>
    struct feature_map_iterator_impl {
        using type = Requirement;
    };

    // We need this to avoid instantiating the default arguments, which could
    // be invalid if `ToVisit` is empty.
    template <typename ToVisit, typename Visited>
    struct lazy_feature_map_iterator_impl {
        using type = feature_map_iterator_impl<ToVisit, Visited>;
    };

    struct end_iterator;

    template <typename ToVisit, typename Visited>
    using feature_map_iterator = typename mpl::if_<
        mpl::empty<ToVisit>,
        mpl::identity<end_iterator>,
        lazy_feature_map_iterator_impl<ToVisit, Visited>
    >::type::type;

    template <typename ToVisit, typename Visited, typename Requirement, bool has_default>
    struct next;

    template <typename ToVisit, typename Visited, typename Requirement>
    struct next<ToVisit, Visited, Requirement, true> {
        using Computation = typename bind<
            typename default_of<Requirement>::type, Requirements...
        >::type;
        using VisitedNow = typename mpl::insert<Visited, Requirement>::type;
        using ToVisitNow = typename mpl::set_insert_range<
            typename mpl::erase_key<ToVisit, Requirement>::type,
            mpl::filter_view<
                typename requirements_of<Computation>::type,
                mpl::not_<mpl::has_key<VisitedNow, mpl::_1>>
            >
        >::type;

        using type = feature_map_iterator<ToVisitNow, VisitedNow>;
    };

    template <typename ToVisit, typename Visited, typename Requirement>
    struct next<ToVisit, Visited, Requirement, false> {
        using ToVisitNow = typename mpl::erase_key<ToVisit, Requirement>::type;
        using VisitedNow = typename mpl::insert<Visited, Requirement>::type;
        using type = feature_map_iterator<ToVisitNow, VisitedNow>;
    };

    template <typename ...Requirements>
    struct all_requirements {
        using type = mpl::iterator_range<
            feature_map_iterator<
                typename mpl::set<Requirements...>::type, mpl::set0<>
            >,
            end_iterator
        >;
    };

    ///////////

    /*!
     *
     * @internal
     * The elements of the `feature_map` are @ref Requirement "Requirements".
     *
     * @todo
     * `static_assert` that we don't have duplicate features with
     * different implementations.
     */
    template <typename ...Requirements>
    struct feature_map {
    private:
        friend mpl::begin<feature_map>;
        using begin = iterator<
            typename mpl::set<Requirements...>::type, mpl::set0<>
        >;

        friend mpl::end<feature_map>;
        using end = iterator<mpl::set0<>, mpl::set0<>>;
    };

    template <typename ...Requirements>
    struct begin<react::feature_map<Requirements...>>
        : identity<
            typename react::feature_map<Requirements...>::begin
        >
    { };

    template <typename ...Requirements>
    struct end<react::feature_map<Requirements...>>
        : identity<
            typename react::feature_map<Requirements...>::end
        >
    { };

    template <typename ...Requirements, typename Requirement>
    struct key_type<react::feature_map<Requirements...>, Requirement>
        : react::feature_of<Requirement>
    { };

    template <typename ...Requirements, typename Requirement>
    struct value_type<react::feature_map<Requirements...>, Requirement>
        : at<react::feature_map<Requirements...>,
            typename react::feature_of<Requirement>::type
        >
    { };

    template <typename ...Requirements, typename Feature>
    struct at<react::feature_map<Requirements...>, Feature> {
    private:
        using First = feature_map_iterator<
            typename mpl::set<Requirements...>::type, mpl::set0<>
        >;
        using Last = end_iterator;

        using AllRequirements = mpl::iterator_range<First, Last>;

    };
        : react::bind<
            if_<>
            react::default_of<Requirement>
            Requirements...
        >
    { };

    template <typename ...Requirements, typename Feature>
    struct has_key<react::feature_map<Requirements...>, Feature>
        : any_of<key_view<react::feature_map<Requirements...>>,
            is_same<Feature, _1>
        >
    { };





    template <typename ...Features>
    struct feature_map {
        template <typename Feature>
        struct at;

        using tag = feature_map;

    private:


        template <typename Feature>
        struct implementation_of
            : mpl::lazy_at_default<
                ProvidedImplementations,
                Feature,
                computation_of<Feature>
            >
        { };

        template <typename Feature>
        struct has_implementation
            : mpl::or_<
                has_default_implementation<Feature>,
                mpl::has_key<ProvidedImplementations, Feature>
            >
        { };

        struct end_iterator;

        template <typename ToVisit, typename Visited>
        struct iterator;

        template <typename ToVisitNow, typename VisitedNow>
        struct make_iterator
            : mpl::if_<mpl::empty<ToVisitNow>,
                end_iterator,
                iterator<ToVisitNow, VisitedNow>
            >
        { };

        template <typename ToVisit, typename Visited>
        struct iterator {
            using Feature = typename mpl::front<ToVisit>::type;
            using Computation = typename at<Feature>::type;
            using VisitedNow = typename mpl::insert<Visited, Feature>::type;
            using ToVisitNow = typename mpl::set_insert_range<
                typename mpl::erase_key<ToVisit, Feature>::type,
                mpl::filter_view<
                    typename requirements_of<Computation>::type,
                    mpl::not_<mpl::has_key<VisitedNow, mpl::_1>>
                >
            >::type;

            using type = typename implementation_of<Feature>::type;
            using next = typename make_iterator<ToVisitNow, VisitedNow>::type;
        };

    public:
        // ForwardSequence
        struct begin
            : make_iterator<
                typename mpl::set<
                    typename feature_of<Implementations>::type...
                >::type,
                typename mpl::set<>::type
            >
        { };

        struct end
            : make_iterator<
                typename mpl::set<>::type,
                typename mpl::set<>::type
            >
        { };

        // AssociativeSequence
        template <typename Feature>
        struct at
            : instantiate<
                typename implementation_of<Feature>::type,
                feature_map
            >
        { };

        template <typename Feature>
        struct has_key
            : has_implementation<Feature>
        { };

        template <typename Implementation>
        struct key_type
            : feature_of<Implementation>
        { };

        template <typename Implementation>
        struct value_type
            : at<typename key_type<Implementation>::type>
        { };

        // ExtensibleAssociativeSequence (not providing erase and erase_key)
        template <typename IgnoredPos, typename Implementation>
        struct insert {
            using type = feature_map<Implementation, Implementations...>;
        };

        struct clear {
            using type = feature_map<>;
        };
    };
} // end namespace feature_map_detail
} // end namespace react

namespace boost { namespace mpl {
    template <typename ...X>
    struct begin<react::feature_map<X...>>
        : react::feature_map_detail::iterator<

        >
    { };

    template <typename ...X>
    struct end<react::feature_map<X...>> {

    };

    template <typename ...X, typename Feature>
    struct at<react::feature_map<X...>, Feature> {

    };

    template <typename ...X, typename Feature>
    struct has_key<react::feature_map<X...>, Feature> {

    };

    template <typename ...X, typename E>
    struct key_type<react::feature_map<X...>, E> {

    };

    template <typename ...X, typename E>
    struct value_type<react::feature_map<X...>, E> {

    };
}} // end namespace boost::mpl
#endif


#endif // !REACT_FEATURE_MAP_HPP
