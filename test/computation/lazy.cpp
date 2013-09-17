/*!
 * @file
 * Contains unit tests for `react::computation::lazy`.
 */

#include <react/computation/lazy.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::lazy<computation_archetype<>>
>);


int main() { }
