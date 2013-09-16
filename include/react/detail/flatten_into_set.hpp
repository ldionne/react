/*!
 * @file
 * Defines `react::detail::flatten_into_set`.
 */

#ifndef REACT_DETAIL_FLATTEN_INTO_SET_HPP
#define REACT_DETAIL_FLATTEN_INTO_SET_HPP

#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>


namespace react {
namespace flatten_into_set_detail {
    using namespace boost::mpl;

    template <typename Sequence>
    struct flatten_into_set
        : fold<
            Sequence,
            typename set<>::type,
            if_<is_sequence<_2>,
                set_insert_range<_1, _2>,
                insert<_1, _2>
            >
        >
    { };
} // end namespace flatten_into_set_detail

namespace detail { using flatten_into_set_detail::flatten_into_set; }
} // end namespace react

#endif // !REACT_DETAIL_FLATTEN_INTO_SET_HPP
