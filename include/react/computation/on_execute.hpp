/*!
 * @file
 * This file defines `react::computation::on_execute`.
 */

#ifndef REACT_COMPUTATION_ON_EXECUTE_HPP
#define REACT_COMPUTATION_ON_EXECUTE_HPP

#include <react/computation/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>


namespace react { namespace computation {
    template <typename Function, typename ...Args>
    struct on_execute : depends_on<Function, Args...> {
        template <typename Env>
        static auto execute(detail::dont_care, Env&& env)
        REACT_AUTO_RETURN(
            retrieve<Function>(env)(retrieve<Args>(env)...)
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_ON_EXECUTE_HPP
