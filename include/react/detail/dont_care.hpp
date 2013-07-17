/*!
 * @file
 * This file defines `react::detail::dont_care`.
 */

#ifndef REACT_DETAIL_DONT_CARE_HPP
#define REACT_DETAIL_DONT_CARE_HPP

namespace react { namespace detail {
    struct dont_care {
        template <typename ...Args>
        constexpr dont_care(Args&& ...) { }
    };
}}

#endif // !REACT_DETAIL_DONT_CARE_HPP
