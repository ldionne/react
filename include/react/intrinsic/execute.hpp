/*!
 * @file
 * This file defines the `react::execute` intrinsic.
 */

#ifndef REACT_INTRINSIC_EXECUTE_HPP
#define REACT_INTRINSIC_EXECUTE_HPP

#include <react/detail/auto_return.hpp>
#include <react/tag_of.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react {
namespace extension {
    template <typename Tag, typename Enable = void>
    struct execute_impl {
        template <typename Env>
        static auto call(Env&& env)
        REACT_AUTO_RETURN(
            boost::remove_reference<Env>::type::execute(
                std::forward<Env>(env)
            )
        )

        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            boost::remove_reference<Computation>::type::execute(
                std::forward<Computation>(c), std::forward<Env>(env)
            )
        )
    };
} // end namespace extension

static constexpr struct execute {
    template <typename Env>
    auto operator()(Env&& env) const
    REACT_AUTO_RETURN(
        extension::execute_impl<
            typename tag_of<Env>::type
        >::call(std::forward<Env>(env))
    )

    template <typename Computation, typename Env>
    auto operator()(Computation&& c, Env&& env) const
    REACT_AUTO_RETURN(
        extension::execute_impl<
            typename tag_of<Computation>::type
        >::call(std::forward<Computation>(c), std::forward<Env>(env))
    )
} execute{};
} // end namespace react

#endif // !REACT_INTRINSIC_EXECUTE_HPP
