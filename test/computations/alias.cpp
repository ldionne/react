/*!
 * @file
 * This file contains unit tests for `react::computations::alias`.
 */

#include <react/computations/alias.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

struct implemented_feature : feature_archetype<> { };
struct aliased_feature : feature_archetype<> { };
BOOST_CONCEPT_ASSERT((IncrementalComputation<
    computations::alias<implemented_feature, aliased_feature>,
    default_construct,
    semantic_tags<>,
    dependency_results<>
>));


int main() { }
