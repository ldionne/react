/*!
 * @file
 * This file defines `react::computation_set`.
 */

#ifndef REACT_COMPUTATION_SET_HPP
#define REACT_COMPUTATION_SET_HPP

#include <react/detail/complete_dependencies.hpp>
#include <react/intrinsic/default_implementation_of.hpp>
#include <react/intrinsic/has_default_implementation.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector.hpp>


namespace react {
/*!
 * Alias to a Boost.MPL `AssociativeSequence` containing all the
 * computations required to satisfy the dependencies of a sequence
 * of `Implementation`s.
 *
 * All placeholders in `Implementations...` and their dependencies are
 * substituted.
 *
 * If `has_default_implementation<I>::value` is `true` for any `I` in
 * `Implementations`, then `I` is replaced by
 * `default_implementation_of<I>::type` prior to processing.
 */
template <typename ...Implementations>
using computation_set = typename detail::complete_dependencies<
    typename boost::mpl::vector<
        typename boost::mpl::eval_if<
            has_default_implementation<Implementations>,
            default_implementation_of<Implementations>,
            boost::mpl::identity<Implementations>
        >::type...
    >::type
>::type;
} // end namespace react

#endif // !REACT_COMPUTATION_SET_HPP
