/*!
 * @file
 * Contains unit tests for `react::computation::strict`.
 */

#include <react/computation/strict.hpp>

#include <react/computation/requiring.hpp>
#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

template <int>
struct requirement : feature_archetype<> { };

REACT_CONCEPT_ASSERT(Computation<
    computation::strict<computation_archetype<>>
>);

REACT_CONCEPT_ASSERT(Computation<
    computation::strict<computation::requiring<requirement<1>>>,
    fake_result_of<requirement<1>>::with<copy_constructible_archetype<>>
>);

REACT_CONCEPT_ASSERT(Computation<
    computation::strict<computation::requiring<requirement<1>, requirement<2>>>,
    fake_result_of<requirement<1>>::with<copy_constructible_archetype<>>,
    fake_result_of<requirement<2>>::with<copy_constructible_archetype<>>
>);


int main() { }
