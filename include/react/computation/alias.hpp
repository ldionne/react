/*!
 * @file
 * This file defines `react::computation::alias`.
 */

#ifndef REACT_COMPUTATION_ALIAS_HPP
#define REACT_COMPUTATION_ALIAS_HPP

#include <react/computation/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <utility>


namespace react { namespace computation {
    /*!
     * Computation whose result is that of another computation.
     *
     * @tparam AliasedFeature
     *         The feature of the computation holding the targeted result.
     */
    template <typename AliasedFeature>
    struct alias : depends_on<AliasedFeature> {
        template <typename Env>
        static auto retrieve(detail::dont_care, Env&& env) REACT_AUTO_RETURN(
            react::retrieve<AliasedFeature>(std::forward<Env>(env))
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_ALIAS_HPP
