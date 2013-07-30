/*!
 * @file
 * This file defines `react::computations::alias`.
 */

#ifndef REACT_COMPUTATIONS_ALIAS_HPP
#define REACT_COMPUTATIONS_ALIAS_HPP

#include <react/computations/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>

#include <utility>


namespace react { namespace computations {
    //! Computation implemented by another computation.
    template <typename AliasedComputation>
    struct alias : depends_on<AliasedComputation> {
        template <typename Env>
        static auto retrieve(detail::dont_care, Env&& env) REACT_AUTO_RETURN(
            react::retrieve<AliasedComputation>(std::forward<Env>(env))
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_ALIAS_HPP
