/*!
 * @file
 * This file defines the `react::ComputationName` concept.
 */

#ifndef REACT_CONCEPT_COMPUTATION_NAME_HPP
#define REACT_CONCEPT_COMPUTATION_NAME_HPP

#include <boost/concept/usage.hpp>


namespace react {
/*!
 * Specification of the `ComputationName` concept.
 *
 * A `ComputationName` is an arbitrary type used to represent a `Computation`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `CN`       | A type modeling the `ComputationName` concept
 *
 *
 * ## Valid expressions
 * | Expression                                          | Return type     | Semantics
 * | ----------                                          | -----------     | ---------
 * | `default_implementation_of<CN>::type<sub>opt</sub>` | A `Computation` | Return the default implementation of the computation represented by `CN`. See `default_implementation_of` for details.
 *
 *
 * @tparam CN
 *         The type to be tested for modeling of the `ComputationName` concept.
 */
template <typename CN>
struct ComputationName {
    BOOST_CONCEPT_USAGE(ComputationName) {
        // Optional requirement:
        // using Default = typename default_implementation_of<CN>::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_COMPUTATION_NAME_HPP
