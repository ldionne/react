/*!
 * @file
 * This file defines `react::computation::executed_before`.
 */

#ifndef REACT_COMPUTATION_EXECUTED_BEFORE_HPP
#define REACT_COMPUTATION_EXECUTED_BEFORE_HPP

#include <react/computation/noop.hpp>
#include <react/detail/flatten_into_set.hpp>

#include <boost/mpl/vector.hpp>


namespace react { namespace computation {
    /*!
     * Computation implemented as a `noop` executed before other features.
     *
     * @tparam Successors...
     *         A sequence of `Feature`s to be executed after this computation.
     *         Any Boost.MPL `Sequence` encountered in `Successors...` is
     *         flattened as-if its content was part of `Successors...`.
     */
    template <typename ...Successors>
    struct executed_before : noop {
        using successors = typename detail::flatten_into_set<
            typename boost::mpl::vector<Successors...>::type
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXECUTED_BEFORE_HPP
