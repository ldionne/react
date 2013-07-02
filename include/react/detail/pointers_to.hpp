/*!
 * @file
 * This file defines `react::detail::pointers_to`.
 */

#ifndef REACT_DETAIL_POINTERS_TO_HPP
#define REACT_DETAIL_POINTERS_TO_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/type_traits/add_pointer.hpp>


namespace react { namespace detail {
    template <typename Sequence>
    struct pointers_to
        : boost::mpl::transform_view<
            Sequence,
            boost::add_pointer<boost::mpl::_1>
        >
    { };
}}

#endif // !REACT_DETAIL_POINTERS_TO_HPP
