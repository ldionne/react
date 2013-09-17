/*!
 * @file
 * Contains unit tests for `react::computation::static_`.
 */

#include <react/computation/static.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::static_<
        computation_archetype<default_constructible_archetype<>>
    >
>);


int main() { }
