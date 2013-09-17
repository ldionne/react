/*!
 * @file
 * Contains unit tests for `react::computation::executed_before`.
 */

#include <react/computation/executed_before.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::executed_before<feature_archetype<>>
>);


int main() { }
