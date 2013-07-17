/*!
 * @file
 * This file defines `react::detail::update_computation`.
 */

#ifndef REACT_DETAIL_UPDATE_COMPUTATION_HPP
#define REACT_DETAIL_UPDATE_COMPUTATION_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <utility>


namespace react { namespace detail {
namespace update_computation_detail {
    template <typename Result>
    struct invoke {
        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            std::forward<Computation>(c).update(std::forward<Env>(env))
        )
    };

    template <>
    struct invoke<void> {
        template <typename Computation, typename Env>
        static auto call(Computation const& c, Env const& env)
        REACT_AUTO_RETURN(
            c.update(env), augment(env, c)
        )
    };
} // end namespace update_computation_detail

static constexpr struct {
    template <typename Env, typename Computation>
    auto operator()(Env&& env, Computation&& c) const
    REACT_AUTO_RETURN(
        update_computation_detail::invoke<decltype(
            std::forward<Computation>(c).update(std::forward<Env>(env))
        )>::call(std::forward<Computation>(c), std::forward<Env>(env))
    )

    template <typename Env, typename Computation>
    auto operator()(Env&& env, Computation&& c) const
    REACT_AUTO_RETURN(
        augment(std::forward<Env>(env), std::forward<Computation>(c))
    )
} update_computation{};
}} // end namespace react::detail

#endif // !REACT_DETAIL_UPDATE_COMPUTATION_HPP
