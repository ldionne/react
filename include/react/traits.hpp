/*!
 * @file
 * This file defines the traits used by the library.
 */

#ifndef REACT_TRAITS_HPP
#define REACT_TRAITS_HPP

namespace react {
    template <typename FeatureSet>
    struct computations_of {
        using type = typename FeatureSet::computations;
    };

    template <typename Computation>
    struct dependencies_of {
        using type = typename Computation::dependencies;
    };

    template <typename Computation>
    struct feature_of {
        using type = typename Computation::feature;
    };

    template <typename CompositeFeature>
    struct subfeatures_of {
        using type = typename CompositeFeature::subfeatures;
    };
} // end namespace react

#endif // !REACT_TRAITS_HPP
