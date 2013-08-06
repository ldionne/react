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
namespace extension {
    template <typename Tag, typename Enable = void>
    struct execute;

    template <typename Tag, typename Enable = void>
    struct update;

    template <typename Tag, typename Enable = void>
    struct augment;

    template <typename Tag, typename Enable = void>
    struct retrieve;
}

static constexpr struct {
    template <typename Computation, typename Env>
    auto operator()(Computation&& c, Env&& env) const
    REACT_AUTO_RETURN(
        extension::execute<
            typename tag_of<Computation>::type
        >::call(std::forward<Computation>(c), std::forward<Env>(env))
    )
} execute{};

static constexpr struct {
    template <typename Env>
    auto operator()(Env&& env) const
    REACT_AUTO_RETURN(
        extension::update<
            typename tag_of<Env>::type
        >::call(std::forward<Env>(env))
    )
} update{};

static constexpr struct {
    template <typename Env, typename ...Computations>
    auto operator()(Env&& env, Computations&& ...c) const
    REACT_AUTO_RETURN(
        extension::augment<
            typename tag_of<Env>::type
        >::call(std::forward<Env>(env), std::forward<Computations>(c)...)
    )
} augment{};

template <typename Name, typename Env>
auto retrieve(Env&& env)
REACT_AUTO_RETURN(
    extension::retrieve<
        typename tag_of<Env>::type
    >::template call<Name>(std::forward<Env>(env))
)

template <typename Computation, typename Env>
auto retrieve(Computation&& c, Env&& env)
REACT_AUTO_RETURN(
    extension::retrieve<
        typename tag_of<Computation>::type
    >::call(std::forward<Computation>(c), std::forward<Env>(env))
)
} // end namespace react

// Always provide the default implementations of the intrinsics.
#include <react/detail/default_intrinsics.hpp>

#endif // !REACT_INTRINSICS_HPP
