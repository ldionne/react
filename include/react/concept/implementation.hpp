/*!
 * @file
 * Defines the `react::Implementation` concept.
 */

#ifndef REACT_CONCEPT_IMPLEMENTATION_HPP
#define REACT_CONCEPT_IMPLEMENTATION_HPP

#include <react/concept/assert.hpp>
#include <react/concept/feature.hpp>
#include <react/concept/usage.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/instantiate.hpp>

#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>


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
 * | `M`        | A Boost.MPL `AssociativeSequence`
 *
 *
 * ## Valid expressions
 * | Expression                | Return type
 * | ----------                | -----------
 * | `feature_of<I>::type`     | A `Feature`
 * | `instantiate<I, M>::type` | A `Computation`
 *
 *
 * @tparam I
 *         The type to be tested for modeling of the `Implementation` concept.
 *
 * @tparam C...
 *         A sequence of models of the `Computation` concept representing
 *         computations with which it is possible to instantiate `I`.
 */
template <typename I, typename ...C>
struct Implementation {
    REACT_CONCEPT_USAGE(Implementation) {
        using ImplementedFeature = typename feature_of<I>::type;
        REACT_CONCEPT_ASSERT(Feature<ImplementedFeature>);

        using M = typename boost::mpl::map<
            boost::mpl::pair<typename feature_of<C>::type, C>...
        >::type;
        using Computation = typename instantiate<I, M>::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_IMPLEMENTATION_HPP
