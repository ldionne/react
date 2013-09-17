/*!
 * @file
 * Contains unit tests for `react::computation::custom_stored`.
 */

#include <react/computation/custom_stored.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>
#include <react/detail/dont_care.hpp>


using namespace react;

template <typename T>
struct nullary_storage { T operator()() const; };

template <typename T>
struct unary_storage { T operator()(detail::dont_care) const; };

REACT_CONCEPT_ASSERT(Computation<
    computation::custom_stored<
        computation_archetype<>, nullary_storage<computation_archetype<>>
    >
>);

REACT_CONCEPT_ASSERT(Computation<
    computation::custom_stored<
        computation_archetype<>, unary_storage<computation_archetype<>>
    >
>);


int main() { }
