/*!
 * @file
 * Defines `react::computation_set`.
 */

#ifndef REACT_COMPUTATION_SET_HPP
#define REACT_COMPUTATION_SET_HPP

#include <react/detail/computation_map.hpp>

#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/value_view.hpp>


namespace react {
template <typename ...Implementations>
using computation_set = typename boost::mpl::set_insert_range<
    typename boost::mpl::set<>::type,
    boost::mpl::value_view<detail::computation_map<Implementations...>>
>::type;
} // end namespace react

#endif // !REACT_COMPUTATION_SET_HPP
