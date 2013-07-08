/*!
 * @file
 * This file defines `react::feature_sets::cloaked`.
 */

#ifndef REACT_FEATURE_SETS_CLOAKED_HPP
#define REACT_FEATURE_SETS_CLOAKED_HPP

#include <react/traits.hpp>

#include <boost/mpl/has_key.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace feature_sets {
    template <typename FeatureSet, typename Computation>
    struct cloaked : FeatureSet {
        using FeatureSet::FeatureSet;
        using FeatureSet::operator=;

        template <typename Feature, typename = typename boost::enable_if<
            boost::mpl::has_key<
                typename dependencies_of<Computation>::type, Feature
            >
        >::type>
        auto operator[](Feature const& feature)
        -> decltype(std::declval<FeatureSet&>()[feature]) {
            return static_cast<FeatureSet&>(*this)[feature];
        }
    };
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_CLOAKED_HPP
