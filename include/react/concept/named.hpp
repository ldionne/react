/*!
 * @file
 * This file defines the `react::Named` concept.
 */

#ifndef REACT_CONCEPT_NAMED_HPP
#define REACT_CONCEPT_NAMED_HPP

#include <react/concept/computation_name.hpp>
#include <react/intrinsic/name_of.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>


namespace react {
/*!
 * Specification of the `Named` concept.
 *
 * A `Named` type is simply a type to which a name is associated. The name
 * can be any type modeling the `ComputationName` concept.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `N`        | A type modeling the `Named` concept
 *
 *
 * ## Valid expressions
 * | Expression         | Return type         | Semantics
 * | ----------         | -----------         | ---------
 * | `name_of<N>::type` | A `ComputationName` | The name associated to `N`. See `name_of` for details.
 *
 *
 * @tparam N
 *         The type to be tested for modeling of the `Named` concept.
 */
template <typename N>
struct Named {
    BOOST_CONCEPT_USAGE(Named) {
        using Name = typename name_of<N>::type;
        BOOST_CONCEPT_ASSERT((ComputationName<Name>));
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_NAMED_HPP
