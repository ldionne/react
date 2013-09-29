/*!
 * @file
 * Includes everything in the library.
 */

#ifndef REACT_REACT_HPP
#define REACT_REACT_HPP

/*!
 * @page Concepts
 *
 * @subpage Feature
 * @subpage Requirement
 * @subpage UnboundComputation
 * @subpage Computation
 * @subpage Environment
 */

/*!
 * @page Feature Feature
 *
 * A feature is a simple type tag representing an abstract functionality
 * to be provided by a computation.
 */

/*!
 * @page Requirement Requirement
 *
 * A requirement bundles a feature and an optional default implementation
 * for that feature.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `R`        | An arbitrary @ref Requirement
 * | `Args...`  | Arbitrary types
 *
 *
 * ## Valid expressions
 * | Expression                                   |
 * | ----------                                   |
 * | `default_of<R, Args...>::type`<sub>opt</sub> |
 * | `feature_of<R>::type`                        |
 */

/*!
 * @page UnboundComputation Unbound Computation
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `UC`       | An arbitrary @ref UnboundComputation
 * | `FM`       | An arbitrary feature map
 *
 *
 * ## Valid expressions
 * | Expression                   |
 * | ----------                   |
 * | `bind<UC, FM>::type`         |
 * | `can_be_bound<UC, FM>::type` |
 */

/*!
 * @page Computation Computation
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `C`        | An arbitrary @ref Computation
 * | `c`        | An object of type `C`
 * | `env`      | An arbitrary @ref Environment
 * | `F`        | An arbitrary @ref Feature
 *
 *
 * ## Valid expressions
 * | Expression                 |
 * | ----------                 |
 * | `C::execute(c, env)`       |
 * | `C::retrieve(c, env)`      |
 * | `requirements_of<C>::type` |
 * | `feature_of<C>::type`      |
 */

/*!
 * @page Environment Environment
 */


/*!
 * @page Definitions
 *
 * - The _predecessors_ of a computation are the features whose computation
 *   must be executed before it.
 *
 * - The _successors_ of a computation are the features whose computation
 *   must be executed after it.
 *
 * - The _feature_ of a computation is the feature it implements.
 *
 * - The _requirements_ of a computation are features that must be available
 *   in an environment in order for the computation to be able to execute
 *   or to be retrieved properly.
 */

#include <react/sandbox/after.hpp>
#include <react/sandbox/before.hpp>
#include <react/sandbox/bind.hpp>
#include <react/sandbox/can_be_bound.hpp>
#include <react/sandbox/computation_of.hpp>
#include <react/sandbox/default_of.hpp>
#include <react/sandbox/defaults_to.hpp>
#include <react/sandbox/execute.hpp>
#include <react/sandbox/feature_map.hpp>
#include <react/sandbox/feature_of.hpp>
#include <react/sandbox/implements.hpp>
#include <react/sandbox/requirements_of.hpp>
#include <react/sandbox/requires.hpp>
#include <react/sandbox/retrieve.hpp>

#endif // !REACT_REACT_HPP
