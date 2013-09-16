/*!
 * @file
 * Defines the `react::execute` intrinsic.
 */

#ifndef REACT_INTRINSIC_EXECUTE_HPP
#define REACT_INTRINSIC_EXECUTE_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/strip.hpp>

#include <utility>


namespace react {
namespace extension {
    template <typename T, typename Enable = void>
    struct execute_impl {
        template <typename Env>
        static auto call(Env&& env)
        REACT_AUTO_RETURN(
            detail::strip<Env>::type::execute(std::forward<Env>(env))
        )

        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            detail::strip<Computation>::type::execute(
                std::forward<Computation>(c), std::forward<Env>(env)
            )
        )
    };
} // end namespace extension

#ifdef REACT_DOXYGEN_INVOKED
    /*!
     * @ingroup intrinsics
     *
     * Execute all of the computations of an environment in the right order.
     *
     * The order is such that all the predecessors of a computation are
     * executed before it and all its successors are executed after.
     */
    template <typename Env>
    auto execute(Env&& env);

    /*!
     * @ingroup intrinsics
     *
     * Execute a computation in the given environment.
     */
    template <typename Computation, typename Env>
    auto execute(Computation&& c, Env&& env)
#else
    static constexpr struct execute {
        template <typename Env>
        auto operator()(Env&& env) const
        REACT_AUTO_RETURN(
            extension::execute_impl<
                typename detail::strip<Env>::type
            >::call(std::forward<Env>(env))
        )

        template <typename Computation, typename Env>
        auto operator()(Computation&& c, Env&& env) const
        REACT_AUTO_RETURN(
            extension::execute_impl<
                typename detail::strip<Computation>::type
            >::call(std::forward<Computation>(c), std::forward<Env>(env))
        )
    } execute{};
#endif
} // end namespace react

#endif // !REACT_INTRINSIC_EXECUTE_HPP
