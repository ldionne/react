/*!
 * @file
 * Defines `react::computation_of`.
 */

#ifndef REACT_COMPUTATION_OF_HPP
#define REACT_COMPUTATION_OF_HPP

#include <react/sandbox/feature_of.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/placeholders.hpp>
#include <type_traits>


namespace react {
    /*!
     * Placeholder substituted for the computation implementing `Feature`
     * in a given feature map.
     */
    template <typename Feature>
    struct computation_of {
        template <typename AvailableComputations>
        class apply {
            using Iter = typename boost::mpl::find_if<
                AvailableComputations,
                std::is_same<Feature, feature_of<boost::mpl::_1>>
            >::type;

            static_assert(!std::is_same<Iter, typename boost::mpl::end<AvailableComputations>::type>::value,
            "");
            // static_assert(boost::mpl::has_key<PartialFeatureMap, Feature>::value,
            // "");

        public:
            // using type = typename boost::mpl::at<PartialFeatureMap, Feature>::type;
            using type = typename boost::mpl::deref<Iter>::type;
#if 0
            using FeatureMap = typename LazyFeatureMap::type;
            static_assert(boost::mpl::has_key<FeatureMap, Feature>::value,
            "while trying to substitute the `computation_of` placeholder: "
            "the requested feature is not in the provided feature map");

        public:
            using type = typename boost::mpl::at<FeatureMap, Feature>::type;
#endif
        };
    };
} // end namespace react

#endif // !REACT_COMPUTATION_OF_HPP
