/*!
 * @file
 * Contains unit tests for `react::computation::executed_after`.
 */

#include <react/computation/executed_after.hpp>

#include <react/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::executed_after<feature_archetype<>>
>);


int main() { }
