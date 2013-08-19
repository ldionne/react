/*!
 * @file
 * This file defines the `react::augment` intrinsic.
 */

#ifndef REACT_INTRINSIC_AUGMENT_HPP
#define REACT_INTRINSIC_AUGMENT_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/strip.hpp>

#include <utility>


namespace react {
namespace extension {
    template <typename T, typename Enable = void>
    struct augment_impl {
        template <typename Env, typename ...Computations>
        static auto call(Env&& env, Computations&& ...c)
        REACT_AUTO_RETURN(
            detail::strip<Env>::type::augment(
                std::forward<Env>(env), std::forward<Computations>(c)...
            )
        )
    };
} // end namespace extension

static constexpr struct augment {
    template <typename Env, typename ...Computations>
    auto operator()(Env&& env, Computations&& ...c) const
    REACT_AUTO_RETURN(
        extension::augment_impl<
            typename detail::strip<Env>::type
        >::call(std::forward<Env>(env), std::forward<Computations>(c)...)
    )
} augment{};
} // end namespace react

#endif // !REACT_INTRINSIC_AUGMENT_HPP
