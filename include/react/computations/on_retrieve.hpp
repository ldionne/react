/*!
 * @file
 * This file defines `react::computations::on_retrieve`.
 */

#ifndef REACT_COMPUTATIONS_ON_RETRIEVE_HPP
#define REACT_COMPUTATIONS_ON_RETRIEVE_HPP

#include <react/computations/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>


namespace react { namespace computations {
    template <typename Function, typename ...Args>
    struct on_retrieve : depends_on<Function, Args...> {
        template <typename Env>
        static auto retrieve(detail::dont_care, Env&& env) REACT_AUTO_RETURN(
            react::retrieve<Function>(env)(react::retrieve<Args>(env)...)
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_ON_RETRIEVE_HPP
