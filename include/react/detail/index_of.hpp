/*!
 * @file
 * This file defines `react::detail::index_of`.
 */

#ifndef REACT_DETAIL_INDEX_OF_HPP
#define REACT_DETAIL_INDEX_OF_HPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/find.hpp>


namespace react { namespace detail {
    //! Return the index of an element in a Boost.MPL `ForwardSequence`.
    template <typename ForwardSequence, typename Element>
    struct index_of
        : boost::mpl::distance<
            typename boost::mpl::begin<ForwardSequence>::type,
            typename boost::mpl::find<ForwardSequence, Element>::type
        >
    { };
}} // end namespace react::detail

#endif // !REACT_DETAIL_INDEX_OF_HPP
