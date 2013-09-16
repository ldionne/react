/*!
 * @file
 * Defines `react::computation::executed_after`.
 */

#ifndef REACT_COMPUTATION_EXECUTED_AFTER_HPP
#define REACT_COMPUTATION_EXECUTED_AFTER_HPP

#include <react/computation/noop.hpp>
#include <react/detail/flatten_into_set.hpp>

#include <boost/mpl/vector.hpp>


namespace react { namespace computation {
    /*!
     * Computation implemented as a `noop` executed after other features.
     *
     * @tparam Predecessors...
     *         A sequence of `Feature`s to be executed before this computation.
     *         Any Boost.MPL `Sequence` encountered in `Predecessors...` is
     *         flattened as-if its content was part of `Predecessors...`.
     */
    template <typename ...Predecessors>
    struct executed_after : noop {
        using predecessors = typename detail::flatten_into_set<
            typename boost::mpl::vector<Predecessors...>::type
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXECUTED_AFTER_HPP
