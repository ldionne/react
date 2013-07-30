/*!
 * @file
 * This file defines `react::computations::on_update`.
 */

#ifndef REACT_COMPUTATIONS_ON_UPDATE_HPP
#define REACT_COMPUTATIONS_ON_UPDATE_HPP

#include <react/computations/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>


namespace react { namespace computations {
    template <typename Function, typename ...Args>
    struct on_update : depends_on<Function, Args...> {
        template <typename Env>
        static auto update(detail::dont_care, Env&& env) REACT_AUTO_RETURN(
            retrieve<Function>(env)(retrieve<Args>(env)...)
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_ON_UPDATE_HPP
