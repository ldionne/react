/*!
 * @file
 * This file defines `react::computation::lazy`.
 */

#ifndef REACT_COMPUTATION_LAZY_HPP
#define REACT_COMPUTATION_LAZY_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <utility>


namespace react { namespace computation {
namespace lazy_detail {
    template <typename Computation, typename Self, typename DefaultEnv>
    class defer_retrieval {
        Self self_;
        DefaultEnv default_env_;

    public:
        template <typename S, typename E>
        explicit defer_retrieval(S&& self, E&& env)
            : self_{std::forward<S>(self)},
              default_env_{std::forward<E>(env)}
        { }

        template <typename Env>
        auto operator()(Env&& env) const REACT_AUTO_RETURN(
            Computation::retrieve(self_, std::forward<Env>(env))
        )

        auto operator()() const REACT_AUTO_RETURN(
            Computation::retrieve(self_, default_env_)
        )
    };
} // end namespace lazy_detail

template <typename Computation>
struct lazy : Computation {
    using Computation::Computation;
    using Computation::operator=;

    template <typename Self, typename Env>
    static auto retrieve(Self&& self, Env&& env) REACT_AUTO_RETURN(
        lazy_detail::defer_retrieval<Computation, Self, Env>{
            std::forward<Self>(self), std::forward<Env>(env)
        }
    )
};
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_LAZY_HPP
