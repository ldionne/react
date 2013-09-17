/*!
 * @file
 * Contains unit tests for `react::computation::union_`.
 */

#include <react/computation/union.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


using namespace react;

template <int>
struct always_disabled { };
struct always_enabled : computation_archetype<> { };

REACT_CONCEPT_ASSERT(Computation<
    computation::union_<always_disabled<1>, always_enabled>
>);

REACT_CONCEPT_ASSERT(Computation<
    computation::union_<always_disabled<1>, always_disabled<2>, always_enabled>
>);

REACT_CONCEPT_ASSERT(Computation<
    computation::union_<
        always_disabled<1>, always_disabled<2>,
        always_disabled<3>, always_enabled
    >
>);


int main() { }
