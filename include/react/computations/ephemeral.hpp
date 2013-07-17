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
    template <typename Computation, typename Counter>
    struct impl : Computation {
        using Computation::Computation;
        using Computation::operator=;

        using dependencies = typename boost::mpl::insert<
            typename dependencies_of<Computation>::type, Counter
        >::type;

        // When the counter hits 0, we remove this
        // computation from the environment.
        template <typename Env, typename = typename boost::enable_if_c<
            retrieve<Counter>(std::declval<Env const&>()) == 0
        >::type>
        auto update(Env const& env) REACT_AUTO_RETURN(
            Computation::update(env), env
        )

        template <typename Env, typename = typename boost::disable_if_c<
            retrieve<Counter>(std::declval<Env&&>()) == 0
        >::type>
        auto update(Env&& env) REACT_AUTO_RETURN(
            Computation::update(std::forward<Env>(env))
        )
    };

    template <typename Computation, unsigned Delay>
    struct make_ephemeral {
        struct counter;
        using Counter = named<counter, static_counter<signed long, -Delay>>;
        using Impl = impl<Computation, counter>;

        using type = depends_on<Impl, Counter>;
    };
} // end namespace ephemeral_detail

template <typename Computation, unsigned Delay>
using ephemeral = typename ephemeral_detail::make_ephemeral<
    Computation, Delay
>::type;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_EPHEMERAL_HPP
