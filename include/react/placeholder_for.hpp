/*!
 * @file
 * Defines `react::placeholder_for`.
 */

#ifndef REACT_PLACEHOLDER_FOR_HPP
#define REACT_PLACEHOLDER_FOR_HPP

#include <boost/mpl/assert.hpp>
#include <boost/mpl/at_default.hpp>


namespace react {
    /*!
     * Placeholder with a special meaning when used inside `computation_set`.
     *
     * Its semantics is to be replaced by the `Computation` whose feature is
     * `Feature` inside the `computation_set`.
     */
    template <typename Feature>
    class placeholder_for {
        template <typename ComputationMap, bool always_false = false>
        struct helpful_failure {
            BOOST_MPL_ASSERT_MSG(
                always_false,
THE_COMPUTATION_MAP_IS_MISSING_A_FEATURE_REQUIRED_FOR_PLACEHOLDER_SUBSTITUTION,
                (ComputationMap, Feature)
            );

            static_assert(always_false,
            "The computation map is missing a feature that is required to "
            "perform placeholder substitution. The feature should either "
            "provide a default implementation or the implementation of that "
            "feature should be provided explicitly to the computation map.");

            struct type;
        };

    public:
        template <typename ...Args>
        struct apply;

        template <typename ComputationMap>
        struct apply<ComputationMap>
            : boost::mpl::lazy_at_default<
                ComputationMap, Feature,
                helpful_failure<ComputationMap>
            >
        { };
    };
} // end namespace react

#endif // !REACT_PLACEHOLDER_FOR_HPP
