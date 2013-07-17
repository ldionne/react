/*!
 * @file
 * This file defines `react::computations::initialized_from`.
 */

#ifndef REACT_COMPUTATIONS_INITIALIZED_FROM_HPP
#define REACT_COMPUTATIONS_INITIALIZED_FROM_HPP

#include <react/intrinsics.hpp>

#include <utility>


namespace react { namespace computations {
template <typename Computation, typename Initializer>
struct initialized_from : Computation {
    template <typename Env>
    explicit initialized_from(Env&& env)
        : Computation{retrieve<Initializer>(std::forward<Env>(env))}
    { }

    using Computation::operator=;
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_INITIALIZED_FROM_HPP
