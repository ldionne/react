/*!
 * @file
 * This file contains unit tests for `react::computations::implemented_by`.
 */

#include <react/computations/implemented_by.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((IncrementalComputation<
    computations::implemented_by<feature_archetype<>, feature_archetype<>>,
    default_construct,
    semantic_tags<>,
    dependency_results<>
>));


int main() { }
