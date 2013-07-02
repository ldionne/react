/*!
 * @file
 * This file defines `react::detail::convertible_if`.
 */

#ifndef REACT_DETAIL_CONVERTIBLE_IF_HPP
#define REACT_DETAIL_CONVERTIBLE_IF_HPP

#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>


namespace react { namespace detail {
    template <typename Condition>
    struct convertible_if {
        template <typename T, typename = typename boost::enable_if<
            typename boost::mpl::apply<Condition, T>::type
        >::type>
        operator T() const { return T{}; }
    };
}}

#endif // !REACT_DETAIL_CONVERTIBLE_IF_HPP
