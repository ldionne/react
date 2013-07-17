/*!
 * @file
 * This file defines the intrinsic operations used by the library.
 */

#ifndef REACT_INTRINSICS_HPP
#define REACT_INTRINSICS_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <utility>


namespace react {
namespace extensions {
    template <typename Tag, typename Enable = void>
    struct update;

    template <typename Tag, typename Enable = void>
    struct augment;

    template <typename Tag, typename Enable = void>
    struct retrieve;
}

static constexpr struct {
    template <typename Computations>
    auto operator()(Computations&& computations) const
    REACT_AUTO_RETURN(
        extensions::update<
            typename tag_of<Computations>::type
        >::call(std::forward<Computations>(computations))
    )
} update{};

static constexpr struct {
    template <typename Env, typename ...Computations>
    auto operator()(Env&& env, Computations&& ...c) const
    REACT_AUTO_RETURN(
        extensions::augment<
            typename tag_of<Env>::type
        >::call(std::forward<Env>(env), std::forward<Computations>(c)...)
    )
} augment{};

template <typename Name, typename Computations>
auto retrieve(Computations&& computations)
REACT_AUTO_RETURN(
    extensions::retrieve<
        typename tag_of<Computations>::type
    >::template call<Name>(std::forward<Computations>(computations))
)
} // end namespace react

#endif // !REACT_INTRINSICS_HPP
