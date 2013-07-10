/*!
 * @file
 * This file defines `react::computations::from_fusion_map`.
 */

#ifndef REACT_COMPUTATIONS_FROM_FUSION_MAP_HPP
#define REACT_COMPUTATIONS_FROM_FUSION_MAP_HPP

#include <react/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/implements.hpp>

#include <boost/fusion/include/at_key.hpp>
#include <utility>


namespace react { namespace computations {
template <typename Feature, typename MapFeature, typename Key = Feature>
struct from_fusion_map : implements<Feature>, depends_on<MapFeature> {
    from_fusion_map() = default;

    template <typename FeatureSet>
    auto result(FeatureSet&& fset) REACT_AUTO_RETURN(
        boost::fusion::at_key<Key>(
            std::forward<FeatureSet>(fset)[MapFeature{}])
    )
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_FROM_FUSION_MAP_HPP
