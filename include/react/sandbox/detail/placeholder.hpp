/*!
 * @file
 * Defines placeholder related stuff.
 */

#ifndef REACT_DETAIL_PLACEHOLDER_HPP
#define REACT_DETAIL_PLACEHOLDER_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>


namespace react {
namespace detail {
    template <typename T>
    struct is_placeholder
        : boost::mpl::false_
    { };

    template <typename T>
    struct is_placeholder_expression
        : is_placeholder<T>
    { };
} // end namespace detail

namespace placeholder_detail {
    template <typename ...Ts>
    struct any_is_placeholder_expr
        : boost::mpl::or_<
            detail::is_placeholder_expression<Ts>...,
            // or_ requires at least 2 arguments
            boost::mpl::false_,
            boost::mpl::false_
        >
    { };
} // end namespace placeholder_detail

namespace detail {
    template <template <typename ...> class F, typename ...T>
    struct is_placeholder_expression<F<T...>>
        : boost::mpl::or_<
            is_placeholder<F<T...>>,
            placeholder_detail::any_is_placeholder_expr<T...>
        >
    { };
} // end namespace detail
} // end namespace react

#endif // !REACT_DETAIL_PLACEHOLDER_HPP
