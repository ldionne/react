/*!
 * @file
 * This file defines `react::computations::alias`.
 */

#ifndef REACT_COMPUTATIONS_ALIAS_HPP
#define REACT_COMPUTATIONS_ALIAS_HPP

#include <react/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/implements.hpp>

#include <utility>


namespace react { namespace computations {
    //! Computation implemented by another feature.
    template <typename ImplementedFeature, typename AliasedFeature>
    struct alias : implements<ImplementedFeature>, depends_on<AliasedFeature> {
        alias() = default;

        template <typename FeatureSet>
        auto result(FeatureSet&& deps) REACT_AUTO_RETURN(
            std::forward<FeatureSet>(deps)[AliasedFeature{}]
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_ALIAS_HPP
