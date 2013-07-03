/*!
 * @file
 * This file defines `react::detail::cloaked_feature_set`.
 */

#ifndef REACT_DETAIL_CLOAKED_FEATURE_SET_HPP
#define REACT_DETAIL_CLOAKED_FEATURE_SET_HPP

#include <react/traits.hpp>

#include <boost/mpl/has_key.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace detail {
template <typename FeatureSet, typename Computation>
struct cloaked_feature_set : FeatureSet {
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
}} // end namespace react::detail

#endif // !REACT_DETAIL_CLOAKED_FEATURE_SET_HPP
