/*!
 * @file
 * This file defines `react::computations::implemented_by`.
 */

#ifndef REACT_COMPUTATIONS_IMPLEMENTED_BY_HPP
#define REACT_COMPUTATIONS_IMPLEMENTED_BY_HPP

#include <react/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/implements.hpp>

#include <utility>


namespace react { namespace computations {
//! Computation implemented by another feature.
template <typename Feature, typename Implementation>
struct implemented_by : implements<Feature>, depends_on<Implementation> {
    implemented_by() = default;

    template <typename FeatureSet>
    auto result(FeatureSet&& fset) REACT_AUTO_RETURN(
        std::forward<FeatureSet>(fset)[Implementation{}]
    )
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_IMPLEMENTED_BY_HPP
