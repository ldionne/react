/*!
 * @file
 * This file defines `react::computation::serialize`.
 */

#ifndef REACT_COMPUTATION_SERIALIZE_HPP
#define REACT_COMPUTATION_SERIALIZE_HPP

#include <react/computation/depends_on.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/retrieve.hpp>


namespace react { namespace computation {
    template <typename Value, typename Archive>
    struct serialize : depends_on<Value, Archive> {
        template <typename Env>
        static void execute(detail::dont_care, Env&& env) {
            retrieve<Archive>(env) & retrieve<Value>(env);
        }
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_SERIALIZE_HPP
