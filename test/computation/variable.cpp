/*!
 * @file
 * Contains unit tests for `react::computation::variable`.
 */

#include <react/computation/variable.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

template <typename T>
struct test_concept
    : Computation<computation::variable<T>>,
      Computation<computation::variable<T*>>,
      Computation<computation::variable<T* const>>,
      Computation<computation::variable<T&>>
{ };

using T = copy_constructible_archetype<>;
REACT_CONCEPT_ASSERT(test_concept<T>);
REACT_CONCEPT_ASSERT(test_concept<T const>);
REACT_CONCEPT_ASSERT(test_concept<T()>);
REACT_CONCEPT_ASSERT(test_concept<T(T)>);
REACT_CONCEPT_ASSERT(test_concept<T[]>);
REACT_CONCEPT_ASSERT(test_concept<T[2]>);
REACT_CONCEPT_ASSERT(test_concept<T[2][4]>);
REACT_CONCEPT_ASSERT(test_concept<T[][2][4]>);


int main() { }
