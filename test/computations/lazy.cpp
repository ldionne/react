/*!
 * @file
 * This file contains unit tests for `react::computations::lazy`.
 */

#include <react/computations/lazy.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((IncrementalComputation<
    computations::lazy<incremental_computation_archetype<>>,
    default_construct,
    semantic_tags<boost::null_archetype<>>,
    dependency_results<>
>));


int main() { }
