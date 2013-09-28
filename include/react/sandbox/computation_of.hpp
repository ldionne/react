/*!
 * @file
 * Defines `react::computation_of`.
 */

#ifndef REACT_COMPUTATION_OF_HPP
#define REACT_COMPUTATION_OF_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>


namespace react {
    /*!
     * Placeholder substituted for the computation implementing `Feature`
     * in a given feature map.
     */
    template <typename Feature>
    struct computation_of {
        template <typename LazyFeatureMap>
        class apply {
            using FeatureMap = typename LazyFeatureMap::type;
            static_assert(boost::mpl::has_key<FeatureMap, Feature>::value,
            "while trying to substitute the `computation_of` placeholder: "
            "the requested feature is not in the provided feature map");

        public:
            using type = typename boost::mpl::at<FeatureMap, Feature>::type;
        };
    };
} // end namespace react

#endif // !REACT_COMPUTATION_OF_HPP
