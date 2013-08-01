/*!
 * @file
 * This file defines all the lambda placeholders provided by the library.
 */

#ifndef REACT_PLACEHOLDERS_HPP
#define REACT_PLACEHOLDERS_HPP

namespace react { namespace placeholders {
    /*!
     * Placeholder replaced by the dependencies of the expression it appears
     * in.
     *
     * Specifically, a Boost.MPL `AssociativeSequence` mapping the name of
     * each dependency to the `Computation` implementing that dependency is
     * substituted in place of the placeholder.
     */
    struct dependencies;
}} // end namespace react::placeholders

#endif // !REACT_PLACEHOLDERS_HPP
