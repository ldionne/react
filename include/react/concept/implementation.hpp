/*!
 * @file
 * This file defines the `react::Implementation` concept.
 */

#ifndef REACT_CONCEPT_IMPLEMENTATION_HPP
#define REACT_CONCEPT_IMPLEMENTATION_HPP

#include <react/concept/feature.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>


namespace react {
/*!
 * Specification of the `Implementation` concept.
 *
 * An `Implementation` is simply a type to which a `Feature` is associated.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `I`        | A type modeling the `Implementation` concept
 *
 *
 * ## Valid expressions
 * | Expression            | Return type | Semantics
 * | ----------            | ----------- | ---------
 * | `feature_of<I>::type` | A `Feature` | The feature implemented by `I`. See `feature_of` for details.
 *
 *
 * @tparam I
 *         The type to be tested for modeling of the `Implementation` concept.
 */
template <typename I>
struct Implementation {
    BOOST_CONCEPT_USAGE(Implementation) {
        using ImplementedFeature = typename feature_of<I>::type;
        BOOST_CONCEPT_ASSERT((Feature<ImplementedFeature>));
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_IMPLEMENTATION_HPP
