/*!
 * @file
 * Contains unit tests for `react::computation::alias`.
 */

#include <react/computation/alias.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::alias<feature_archetype<>>,
    fake_result_of<
        feature_archetype<>
    >::with<
        copy_constructible_archetype<>
    >
>);


int main() { }
