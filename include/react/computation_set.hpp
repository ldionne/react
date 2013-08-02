/*!
 * @file
 * This file defines `react::computation_set`.
 */

#ifndef REACT_COMPUTATION_SET_HPP
#define REACT_COMPUTATION_SET_HPP

#include <react/detail/complete_dependencies.hpp>
#include <react/detail/bind.hpp>

#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
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
    using computation_set = typename boost::mpl::set_insert_range<
        boost::mpl::set<>,
        typename detail::bind<
            typename detail::complete_dependencies<
                typename boost::mpl::vector<Computations...>::type
            >::type
        >::type
    >::type;
} // end namespace react

#endif // !REACT_COMPUTATION_SET_HPP
