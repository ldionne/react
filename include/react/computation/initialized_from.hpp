/*!
 * @file
 * This file defines `react::computation::initialized_from`.
 */

#ifndef REACT_COMPUTATION_INITIALIZED_FROM_HPP
#define REACT_COMPUTATION_INITIALIZED_FROM_HPP

#include <react/intrinsic/retrieve.hpp>

#include <utility>


namespace react { namespace computation {
template <typename Computation, typename Initializer>
struct initialized_from : Computation {
    template <typename Env>
    explicit initialized_from(Env&& env)
        : Computation(retrieve<Initializer>(std::forward<Env>(env)))
    { }

    using Computation::operator=;
};
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_INITIALIZED_FROM_HPP
