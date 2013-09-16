/*!
 * @file
 * Contains unit tests for the concept archetypes.
 */

#include <react/archetypes.hpp>

#include <react/concepts.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<computation_archetype<>>);
REACT_CONCEPT_ASSERT(Feature<feature_archetype<>>);
REACT_CONCEPT_ASSERT(Environment<environment_archetype<>>);
REACT_CONCEPT_ASSERT(Implementation<implementation_archetype<>>);


int main() { }
