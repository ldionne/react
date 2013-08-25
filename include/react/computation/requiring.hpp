/*!
 * @file
 * This file defines `react::computation::requiring`.
 */

#ifndef REACT_COMPUTATION_REQUIRING_HPP
#define REACT_COMPUTATION_REQUIRING_HPP

#include <react/computation/noop.hpp>
#include <react/detail/flatten_into_set.hpp>

#include <boost/mpl/vector.hpp>


namespace react { namespace computation {
    /*!
     * Computation implemented as a `noop` requiring other features.
     *
     * @tparam Requirements...
     *         A sequence of `Feature`s required by this computation. Any
     *         Boost.MPL `Sequence` encountered in `Requirements...` is
     *         flattened as-if its content was part of `Requirements...`.
     */
    template <typename ...Requirements>
    struct requiring : noop {
        using requirements = typename detail::flatten_into_set<
            typename boost::mpl::vector<Requirements...>::type
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_REQUIRING_HPP
