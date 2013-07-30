/*!
 * @file
 * This file defines `react::detail::update_computation`.
 */

#ifndef REACT_DETAIL_UPDATE_COMPUTATION_HPP
#define REACT_DETAIL_UPDATE_COMPUTATION_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace detail {
static constexpr class {
    template <typename Env, typename Comp>
    static auto call(Env&& env, Comp&& c, int)
    REACT_AUTO_RETURN(
        typename boost::enable_if<boost::is_void<decltype(
            c.update(env)
        )>>::type(),
        c.update(env),
        augment(std::forward<Env>(env), std::forward<Comp>(c))
    )

    template <typename Env, typename Comp>
    static auto call(Env&& env, Comp&& c, int)
    REACT_AUTO_RETURN(
        typename boost::disable_if<boost::is_void<decltype(
            std::forward<Comp>(c).update(std::forward<Env>(env))
        )>>::type(),
        std::forward<Comp>(c).update(std::forward<Env>(env))
    )

    // This overload is picked if both overloads with int are invalid.
    template <typename Env, typename Comp>
    static auto call(Env&& env, Comp&& c, void*)
    REACT_AUTO_RETURN(
        augment(std::forward<Env>(env), std::forward<Comp>(c))
    )

public:
    template <typename Env, typename Computation>
    auto operator()(Env&& env, Computation&& c) const
    REACT_AUTO_RETURN(
        call(std::forward<Env>(env), std::forward<Computation>(c), (int)0)
    )
} update_computation{};
}} // end namespace react::detail

#endif // !REACT_DETAIL_UPDATE_COMPUTATION_HPP
