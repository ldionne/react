/*!
 * @file
 * This file defines `react::computations::lazy`.
 */

#ifndef REACT_COMPUTATIONS_LAZY_HPP
#define REACT_COMPUTATIONS_LAZY_HPP

#include <react/detail/auto_return.hpp>

#include <utility>


namespace react { namespace computations {
namespace lazy_detail {
    template <typename DefaultEnv, typename Computation>
    class defer_retrieval {
        DefaultEnv& default_env_;
        Computation& computation_;

    public:
        explicit defer_retrieval(DefaultEnv& env, Computation& c)
            : default_env_{env}, computation_{c}
        { }

        template <typename Env>
        auto operator()(Env&& env) REACT_AUTO_RETURN(
            computation_.retrieve(std::forward<Env>(env))
        )

        auto operator()() REACT_AUTO_RETURN(
            computation_.retrieve(default_env_)
        )
    };
} // end namespace lazy_detail

template <typename Computation>
struct lazy : Computation {
    using Computation::Computation;
    using Computation::operator=;

    template <typename Env>
    lazy_detail::defer_retrieval<Env const, Computation>
    retrieve(Env const& env) {
        return {env, *this};
    }

    template <typename Env>
    lazy_detail::defer_retrieval<Env const, Computation const>
    retrieve(Env const& env) const {
        return {env, *this};
    }
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_LAZY_HPP
