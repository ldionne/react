/*!
 * @file
 * Defines `react::computation::extern_`.
 */

#ifndef REACT_COMPUTATION_EXTERN_HPP
#define REACT_COMPUTATION_EXTERN_HPP

#include <react/computation/implementing.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/mpl/assert.hpp>


namespace react { namespace computation {
    /*!
     * Placeholder to allow late-binding of a computation to a feature.
     *
     * @note
     * This placeholder is useful when used in conjunction with the rebinding
     * semantics of `augment`. Whenever desired, the feature may be rebound
     * to a different computation in an environment by using `augment`.
     *
     * @note
     * Technically, `extern_` is not a model of the `Computation` concept
     * because `retrieve(an_extern_computation, any_environment)` is
     * ill-formed. This is by design.
     *
     * @tparam Feature
     *         The feature whose implementation is to be late-bound.
     */
    template <typename Feature>
    struct extern_ : implementing<Feature> {
        template <bool always_false = false>
        static void retrieve(detail::dont_care, detail::dont_care) {
            BOOST_MPL_ASSERT_MSG(always_false,
                TRYING_TO_RETRIEVE_THE_RESULT_OF_AN_EXTERN_COMPUTATION,
                (Feature)
            );
            static_assert(always_false,
                "trying to retrieve the result of an `extern_` computation");
        }
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXTERN_HPP
