/*!
 * @file
 * This file defines `react::detail::none_of`.
 */

#ifndef REACT_DETAIL_NONE_OF_HPP
#define REACT_DETAIL_NONE_OF_HPP

#include <boost/mpl/end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/type_traits/is_same.hpp>


namespace react { namespace detail {
    template <typename Sequence, typename Pred>
    struct none_of
        : boost::is_same<
            typename boost::mpl::end<Sequence>::type,
            typename boost::mpl::find_if<Sequence, Pred>::type
        >
    { };
}}

#endif // !REACT_DETAIL_NONE_OF_HPP
