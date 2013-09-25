/*!
 * @file
 * Defines the `react::instantiate` intrinsic.
 */

#ifndef REACT_INTRINSIC_INSTANTIATE_HPP
#define REACT_INTRINSIC_INSTANTIATE_HPP

#include <react/detail/either.hpp>
#include <react/detail/strip.hpp>
#include <react/placeholder_for.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>


namespace react {
namespace instantiate_detail {
    template <typename T>
    struct is_placeholder
        : boost::mpl::false_
    { };

    template <typename Feature>
    struct is_placeholder<placeholder_for<Feature>>
        : boost::mpl::true_
    { };

    template <typename T>
    struct is_placeholder_expr
        : is_placeholder<T>
    { };

    template <typename ...Ts>
    struct any_is_placeholder_expr
        : boost::mpl::or_<
            is_placeholder_expr<Ts>...,
            // or_ requires at least 2 arguments
            boost::mpl::false_,
            boost::mpl::false_
        >
    { };

    template <template <typename ...> class F, typename ...T>
    struct is_placeholder_expr<F<T...>>
        : boost::mpl::or_<
            is_placeholder<F<T...>>,
            any_is_placeholder_expr<T...>
        >
    { };
} // end namespace instantiate_detail

namespace extension {
    template <typename RawImplementation, typename Enable = void>
    struct instantiate_impl {
        template <typename, typename ComputationMap>
        struct apply {
            template <typename T>
            struct replace
                : boost::mpl::identity<T>
            { };

            template <template <typename ...> class F, typename ...T>
            struct replace<F<T...>>
                : boost::mpl::if_<
                    instantiate_detail::any_is_placeholder_expr<T...>,
                    detail::either<
                        F<typename replace<T>::type...>,
                        boost::mpl::identity<F<typename replace<T>::type...>>
                    >,
                    boost::mpl::identity<F<T...>>
                >::type
            { };

            template <typename Feature>
            struct replace<placeholder_for<Feature>>
                : placeholder_for<Feature>::template apply<ComputationMap>
            { };

            using type = typename replace<RawImplementation>::type;
        };
    };
} // end namespace extension

/*!
 * @ingroup intrinsics
 *
 * The computation implementing the feature of `Implementation`,
 * parameterized with the computations in `ComputationMap`.
 */
template <typename Implementation, typename ComputationMap>
struct instantiate
    : extension::instantiate_impl<
        typename detail::strip<Implementation>::type
    >::template apply<Implementation, ComputationMap>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_INSTANTIATE_HPP
