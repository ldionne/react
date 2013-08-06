/*!
 * @file
 * This file defines the `react::retrieve` intrinsic.
 */

#ifndef REACT_INTRINSIC_RETRIEVE_HPP
#define REACT_INTRINSIC_RETRIEVE_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react {
namespace extension {
    template <typename Tag, typename Enable = void>
    struct retrieve {
        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            boost::remove_reference<Computation>::type::retrieve(
                std::forward<Computation>(c), std::forward<Env>(env)
            )
        )

        template <typename Name, typename Env, bool always_false = false>
        static void call(Env&&) {
            static_assert(always_false,
                "There is no default implementation for "
                "`react::retrieve<ComputationName>(Environment)`.");
        }
    };
} // end namespace extension

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

#endif // !REACT_INTRINSIC_RETRIEVE_HPP
