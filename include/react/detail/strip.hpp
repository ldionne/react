/*!
 * @file
 * Defines `react::detail::strip`.
 */

#ifndef REACT_DETAIL_STRIP_HPP
#define REACT_DETAIL_STRIP_HPP

#include <type_traits>


namespace react { namespace detail {
    template <typename T>
    struct strip
        : std::remove_cv<
            typename std::remove_reference<T>::type
        >
    { };
}} // end namespace react::detail

#endif // !REACT_DETAIL_STRIP_HPP
