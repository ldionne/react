/*!
 * @file
 * Defines `react::computation_of`.
 */

#ifndef REACT_COMPUTATION_OF_HPP
#define REACT_COMPUTATION_OF_HPP

#include <react/sandbox/can_be_bound.hpp>
#include <react/sandbox/detail/placeholder.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_key.hpp>


namespace react {
    /*!
     * Unbound computation and placeholder substituted for the computation
     * implementing `Feature` in a given feature map.
     *
     * @todo
     * The check that `Feature` is in the `FeatureMap` should be unnecessary,
     * since we guarantee that `computation_of` is only applied when
     * `can_be_bound` is true.
     */
    template <typename Feature>
    struct computation_of {
        template <typename FeatureMap>
        struct apply {
            static_assert(boost::mpl::has_key<FeatureMap, Feature>::value,
            "while trying to substitute the `computation_of` placeholder: "
            "the requested feature is not in the provided feature map");

            using type = typename boost::mpl::at<FeatureMap, Feature>::type;
        };
    };

    template <typename Feature, typename FeatureMap>
    struct can_be_bound<computation_of<Feature>, FeatureMap>
        : boost::mpl::has_key<FeatureMap, Feature>
    { };

    namespace detail {
        template <typename Feature>
        struct is_placeholder<computation_of<Feature>>
            : boost::mpl::true_
        { };
    }
} // end namespace react

#endif // !REACT_COMPUTATION_OF_HPP
