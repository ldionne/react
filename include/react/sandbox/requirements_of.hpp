/*!
 * @file
 * Defines `react::requirements_of`.
 */

#ifndef REACT_REQUIREMENTS_OF_HPP
#define REACT_REQUIREMENTS_OF_HPP

#include <react/sandbox/detail/fetch_nested.hpp>

#include <boost/mpl/set.hpp>


namespace react {
#ifdef REACT_DOXYGEN_INVOKED
    /*!
     * Returns the requirements of a computation.
     *
     * If the computation does not have a nested `requirements` type,
     * the computation has no requirements.
     *
     * @return A Boost.MPL AssociativeSequence of
     *         @ref Requirement "Requirements"
     */
    template <typename Computation>
    struct requirements_of { };
#else
    REACT_FETCH_NESTED(requirements_of, requirements, boost::mpl::set0<>)
#endif
} // end namespace react

#endif // !REACT_REQUIREMENTS_OF_HPP
