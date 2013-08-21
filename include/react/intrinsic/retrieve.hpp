/*!
 * @file
 * This file defines the `react::retrieve` intrinsic.
 */

#ifndef REACT_INTRINSIC_RETRIEVE_HPP
#define REACT_INTRINSIC_RETRIEVE_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/strip.hpp>

#include <utility>


namespace react {
namespace extension {
    template <typename T, typename Enable = void>
    struct retrieve_impl {
        template <typename Feature, typename Env>
        static auto call(Env&& env)
        REACT_AUTO_RETURN(
            detail::strip<Env>::type::template retrieve<Feature>(
                std::forward<Env>(env)
            )
        )

        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            detail::strip<Computation>::type::retrieve(
                std::forward<Computation>(c), std::forward<Env>(env)
            )
        )
    };
} // end namespace extension

template <typename Feature, typename Env>
auto retrieve(Env&& env)
REACT_AUTO_RETURN(
    extension::retrieve_impl<
        typename detail::strip<Env>::type
    >::template call<Feature>(std::forward<Env>(env))
)

template <typename Computation, typename Env>
auto retrieve(Computation&& c, Env&& env)
REACT_AUTO_RETURN(
    extension::retrieve_impl<
        typename detail::strip<Computation>::type
    >::call(std::forward<Computation>(c), std::forward<Env>(env))
)
} // end namespace react

#endif // !REACT_INTRINSIC_RETRIEVE_HPP
