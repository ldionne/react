/*!
 * @file
 * This file defines `react::computations::ephemeral`.
 */

#ifndef REACT_COMPUTATIONS_EPHEMERAL_HPP
#define REACT_COMPUTATIONS_EPHEMERAL_HPP

#include <react/computations/depends_on.hpp>
#include <react/computations/named.hpp>
#include <react/computations/static_counter.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>
#include <react/traits.hpp>

#include <boost/mpl/insert.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace computations {
namespace ephemeral_detail {
    template <typename Computation, typename Counter, unsigned long Delay>
    struct impl : Computation {
        using Computation::Computation;
        using Computation::operator=;

        using dependencies = typename boost::mpl::insert<
            typename dependencies_of<Computation>::type, Counter
        >::type;

        // We remove this computation from the environment when
        // the time is elapsed.
        template <typename Self, typename Env, typename = typename boost::enable_if_c<
            retrieve<Counter>(std::declval<Env&&>()) == Delay
        >::type>
        static auto update(Self&& self, Env&& env) REACT_AUTO_RETURN(
            Computation::update(std::forward<Self>(self), env),
            std::forward<Env>(env)
        )

        template <typename Self, typename Env, typename = typename boost::disable_if_c<
            retrieve<Counter>(std::declval<Env&&>()) == Delay
        >::type>
        static auto update(Self&& self, Env&& env) REACT_AUTO_RETURN(
            Computation::update(
                std::forward<Self>(self), std::forward<Env>(env)
            )
        )
    };

    template <typename Computation, unsigned long Delay>
    struct make_ephemeral {
        struct counter;
        using Counter = named<counter, static_counter<unsigned long, 0>>;
        using Impl = impl<Computation, counter, Delay>;

        using type = depends_on<Impl, Counter>;
    };
} // end namespace ephemeral_detail

template <typename Computation, unsigned long Delay>
using ephemeral = typename ephemeral_detail::make_ephemeral<
    Computation, Delay
>::type;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_EPHEMERAL_HPP
