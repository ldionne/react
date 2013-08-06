/*!
 * @file
 * This file defines the `react::update` intrinsic.
 */

#ifndef REACT_INTRINSIC_UPDATE_HPP
#define REACT_INTRINSIC_UPDATE_HPP

#include <react/detail/auto_return.hpp>
#include <react/tag_of.hpp>

#include <utility>


namespace react {
namespace extension {
    template <typename Tag, typename Enable = void>
    struct update_impl {
        template <typename Env, bool always_false = false>
        static void call(Env&&) {
            static_assert(always_false,
            "There is no default implementation for "
            "`react::update(Environment)`.");
        }
    };
} // end namespace extension

static constexpr struct {
    template <typename Env>
    auto operator()(Env&& env) const
    REACT_AUTO_RETURN(
        extension::update_impl<
            typename tag_of<Env>::type
        >::call(std::forward<Env>(env))
    )
} update{};
} // end namespace react

#endif // !REACT_INTRINSIC_UPDATE_HPP
