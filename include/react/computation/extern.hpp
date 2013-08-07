/*!
 * @file
 * This file defines `react::computation::extern_`.
 */

#ifndef REACT_COMPUTATION_EXTERN_HPP
#define REACT_COMPUTATION_EXTERN_HPP

#include <react/computation/named.hpp>
#include <react/detail/dont_care.hpp>


namespace react { namespace computation {
    template <typename ComputationName>
    struct extern_ : named<ComputationName> {
        template <bool always_false = false>
        static void execute(detail::dont_care, detail::dont_care) {
            static_assert(always_false,
                "trying to execute an `extern_` computation");
        }
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXTERN_HPP
