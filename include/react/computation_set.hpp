/*!
 * @file
 * This file defines `react::computation_set`.
 */

#ifndef REACT_COMPUTATION_SET_HPP
#define REACT_COMPUTATION_SET_HPP

#include <react/detail/complete_dependencies.hpp>

#include <boost/mpl/vector.hpp>


namespace react {
    /*!
     * Alias to a Boost.MPL `AssociativeSequence` containing all the
     * computations required to satisfy the dependencies of a sequence
     * of `Computations`.
     *
     * All placeholders in `Computations` and their dependencies are
     * substituted.
     */
    template <typename ...Computations>
    using computation_set = typename detail::complete_dependencies<
        typename boost::mpl::vector<Computations...>::type
    >::type;
} // end namespace react

#endif // !REACT_COMPUTATION_SET_HPP
