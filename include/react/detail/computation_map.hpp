/*!
 * @file
 * Defines `react::detail::computation_map`.
 */

#ifndef REACT_DETAIL_COMPUTATION_MAP_HPP
#define REACT_DETAIL_COMPUTATION_MAP_HPP

#include <react/intrinsic/default_implementation_of.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/instantiate.hpp>
#include <react/intrinsic/requirements_of.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/at_default.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/preprocessor/cat.hpp>


namespace react {
namespace computation_map_detail {
    namespace mpl = boost::mpl;

    template <typename ...>
    struct always_false {
        static constexpr bool value = false;
    };

    template <typename Feature>
    struct helpful_failure {
        BOOST_MPL_ASSERT_MSG(
            always_false<Feature>::value,
            MISSING_A_DEFAULT_IMPLEMENTATION_FOR_THE_FOLLOWING_FEATURE,
            (Feature)
        );

        static_assert(always_false<Feature>::value,
        "Missing a default implementation for some feature. "
        "The implementation for that feature must be provided "
        "explicitly or the feature must define a default implementation "
        "using the `react::default_implementation_of` intrinsic.");

        struct type;
    };

    template <typename ...Implementations>
    struct computation_map {
        template <typename Feature>
        struct at;

        using tag = computation_map;

    private:
        using ProvidedImplementations = typename mpl::map<
            mpl::pair<
                typename feature_of<Implementations>::type, Implementations
            >...
        >::type;

        template <typename Feature>
        struct implementation_of
            : mpl::lazy_at_default<
                ProvidedImplementations, Feature,
                mpl::eval_if<has_default_implementation<Feature>,
                    default_implementation_of<Feature>,
                    helpful_failure<Feature>
                >
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
                computation_map
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
            using type = computation_map<Implementation, Implementations...>;
        };

        struct clear {
            using type = computation_map<>;
        };
    };
} // end namespace computation_map_detail

namespace detail {
    /*!
     * Compile-time map associating a feature to the computation
     * implementing it.
     *
     * @tparam ImplementationsOrFeatures...
     *         A sequence of `Implementation`s or `Feature`s from which the
     *         map shall be generated. Elements in `ImplementationsOrFeatures`
     *         that have a default implementation are replaced by that default
     *         implementation prior to processing.
     */
    template <typename ...ImplementationsOrFeatures>
    using computation_map = computation_map_detail::computation_map<
        typename boost::mpl::eval_if<
            has_default_implementation<ImplementationsOrFeatures>,
            default_implementation_of<ImplementationsOrFeatures>,
            boost::mpl::identity<ImplementationsOrFeatures>
        >::type...
    >;
} // end namespace detail
} // end namespace react

#define REACT_FORWARD_TO_COMPUTATION_MAP(intrinsic)                         \
    template <typename ...Implementations>                                  \
    struct BOOST_PP_CAT(intrinsic, _impl)<                                  \
        react::computation_map_detail::computation_map<Implementations...>  \
    >                                                                       \
    {                                                                       \
        template <typename Self, typename ...Args>                          \
        struct apply                                                        \
            : Self::template intrinsic<Args...>                             \
        { };                                                                \
    };                                                                      \
/**/

#define REACT_FORWARD_TO_COMPUTATION_MAP_NULLARY(intrinsic)                 \
    template <typename ...Implementations>                                  \
    struct BOOST_PP_CAT(intrinsic, _impl)<                                  \
        react::computation_map_detail::computation_map<Implementations...>  \
    >                                                                       \
    {                                                                       \
        template <typename Self>                                            \
        struct apply                                                        \
            : Self::intrinsic                                               \
        { };                                                                \
    };                                                                      \
/**/

namespace boost { namespace mpl {
    REACT_FORWARD_TO_COMPUTATION_MAP(at)
    REACT_FORWARD_TO_COMPUTATION_MAP(has_key)
    REACT_FORWARD_TO_COMPUTATION_MAP(key_type)
    REACT_FORWARD_TO_COMPUTATION_MAP(value_type)
    REACT_FORWARD_TO_COMPUTATION_MAP(insert)

    REACT_FORWARD_TO_COMPUTATION_MAP_NULLARY(begin)
    REACT_FORWARD_TO_COMPUTATION_MAP_NULLARY(end)
    REACT_FORWARD_TO_COMPUTATION_MAP_NULLARY(clear)
}}

#undef REACT_FORWARD_TO_COMPUTATION_MAP_NULLARY
#undef REACT_FORWARD_TO_COMPUTATION_MAP

#endif // !REACT_DETAIL_COMPUTATION_MAP_HPP
