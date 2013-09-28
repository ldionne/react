/*!
 * @file
 * Defines `react::feature_of`.
 */

#ifndef REACT_FEATURE_OF_HPP
#define REACT_FEATURE_OF_HPP

#include <react/sandbox/detail/fetch_nested.hpp>


namespace react {
#ifdef REACT_DOXYGEN_INVOKED
    /*!
     * Returns the feature of a computation or requirement.
     *
     * If the `T::feature` expression is invalid, the feature of `T`
     * is `T` itself.
     *
     * @return A @ref Feature
     */
    template <typename T>
    struct feature_of { };
#else
    REACT_FETCH_NESTED(feature_of, feature, U)
#endif

    //! Equivalent to `feature_of<Requirement>`.
    template <typename Requirement, typename ...Args>
    struct feature_of<Requirement(Args...)>
        : feature_of<Requirement>
    { };
} // end namespace react

#endif // !REACT_FEATURE_OF_HPP
