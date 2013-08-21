/*!
 * @file
 * This file defines the `react::ComputationName` concept.
 */

#ifndef REACT_CONCEPT_COMPUTATION_NAME_HPP
#define REACT_CONCEPT_COMPUTATION_NAME_HPP

#include <react/intrinsic/default_implementation_of.hpp>
#include <react/intrinsic/has_default_implementation.hpp>

#include <boost/concept/usage.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/void.hpp>


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
 * | Expression                                          | Return type                            | Semantics
 * | ----------                                          | -----------                            | ---------
 * | `default_implementation_of<CN>::type<sub>opt</sub>` | A `Computation`                        | Return the default implementation of the computation represented by `CN`. See `default_implementation_of` for details.
 * | `has_default_implementation<CN>::type`              | A Boost.MPL boolean `IntegralConstant` | Return whether `CN` has a default implementation. See `has_default_implementation` for details.
 *
 *
 * @tparam CN
 *         The type to be tested for modeling of the `ComputationName` concept.
 */
template <typename CN>
struct ComputationName {
    BOOST_CONCEPT_USAGE(ComputationName) {
        using HasDefault = typename has_default_implementation<CN>::type;

        using Default = typename boost::mpl::eval_if<HasDefault,
            default_implementation_of<CN>,
            boost::mpl::void_
        >::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_COMPUTATION_NAME_HPP
