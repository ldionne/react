/*!
 * @file
 * This file defines `react::detail::any_of`.
 */

#ifndef RECT_DETAIL_ANY_OF_HPP
#define RECT_DETAIL_ANY_OF_HPP

#include <react/detail/none_of.hpp>

#include <boost/mpl/not.hpp>


namespace react { namespace detail {
    template <typename Sequence, typename Pred>
    struct any_of
        : boost::mpl::not_<
            none_of<Sequence, Pred>
        >
    { };
}}

#endif // !RECT_DETAIL_ANY_OF_HPP
