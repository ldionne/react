/*!
 * @file
 * Defines `react::computation_of`.
 */

#ifndef REACT_COMPUTATION_OF_HPP
#define REACT_COMPUTATION_OF_HPP

#include <react/sandbox/can_be_bound.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl11/bool.hpp>
#include <boost/mpl11/functional/is_placeholder.hpp>


namespace react {
    /*!
     * Unbound computation and placeholder substituted for the computation
     * implementing `Feature` in a given feature map.
     */
    template <typename Feature>
    struct computation_of {
        template <typename FeatureMap>
        struct apply
            : boost::mpl::at<FeatureMap, Feature>
        { };
    };

    template <typename Feature, typename FeatureMap>
    struct can_be_bound<computation_of<Feature>, FeatureMap>
        : boost::mpl::has_key<FeatureMap, Feature>
    { };
} // end namespace react

namespace boost { namespace mpl11 {
    template <typename Feature>
    struct is_placeholder<react::computation_of<Feature>>
        : true_
    { };
}}

#endif // !REACT_COMPUTATION_OF_HPP
