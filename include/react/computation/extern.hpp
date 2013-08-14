/*!
 * @file
 * This file defines `react::computation::extern_`.
 */

#ifndef REACT_COMPUTATION_EXTERN_HPP
#define REACT_COMPUTATION_EXTERN_HPP

#include <react/computation/named.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/mpl/assert.hpp>


namespace react { namespace computation {
    /*!
     * Placeholder for a computation whose implementation is not known yet.
     *
     * @note
     * This placeholder is useful when used in conjunction with the rebinding
     * semantics of `augment`. Whenever the implementation of the computation
     * is known, the name of the `extern_` computation may be rebound to the
     * new implementation in an environment by using `augment`.
     *
     * @note
     * Technically, `extern_` is not a model of the `Computation` concept
     * because `retrieve(an_extern_computation, any_environment)` is
     * ill-formed. This is by design.
     *
     * @tparam ComputationName
     *         The name of the computation whose implementation is not known
     *         yet.
     */
    template <typename ComputationName>
    struct extern_ : named<ComputationName> {
        template <bool always_false = false>
        static void retrieve(detail::dont_care, detail::dont_care) {
            BOOST_MPL_ASSERT_MSG(always_false,
                trying_to_retrieve_the_result_of_an_extern_computation,
                (ComputationName)
            );
            static_assert(always_false,
                "trying to retrieve the result of an `extern_` computation");
        }
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXTERN_HPP
