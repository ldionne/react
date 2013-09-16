/*!
 * @file
 * Defines the `react::Feature` concept.
 */

#ifndef REACT_CONCEPT_FEATURE_HPP
#define REACT_CONCEPT_FEATURE_HPP

#include <react/concept/usage.hpp>
#include <react/intrinsic/default_implementation_of.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/void.hpp>


namespace react {
/*!
 * Specification of the `Feature` concept.
 *
 * A `Feature` represents an abstract functionality provided by a
 * `Computation`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `F`        | A type modeling the `Feature` concept
 *
 *
 * ## Valid expressions
 * | Expression                                         | Return type
 * | ----------                                         | -----------
 * | `default_implementation_of<F>::type<sub>opt</sub>` | A `Computation`
 *
 *
 * @tparam F
 *         The type to be tested for modeling of the `Feature` concept.
 */
template <typename F>
struct Feature {
    REACT_CONCEPT_USAGE(Feature) {
        namespace mpl = boost::mpl;

        using Default = typename mpl::eval_if<has_default_implementation<F>,
            default_implementation_of<F>,
            mpl::void_
        >::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_FEATURE_HPP
