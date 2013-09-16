/*!
 * @file
 * Defines `react::detail::strip`.
 */

#ifndef REACT_DETAIL_STRIP_HPP
#define REACT_DETAIL_STRIP_HPP

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react { namespace detail {
    template <typename T>
    struct strip
        : boost::remove_cv<
            typename boost::remove_reference<T>::type
        >
    { };
}} // end namespace react::detail

#endif // !REACT_DETAIL_STRIP_HPP
