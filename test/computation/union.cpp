/*!
 * @file
 * This file contains unit tests for `react::computation::union_`.
 */

#include <react/computation/union.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

template <int>
struct always_disabled { };
struct always_enabled : computation_archetype<> { };

BOOST_CONCEPT_ASSERT((Computation<
    computation::union_<always_disabled<1>, always_enabled>
>));

BOOST_CONCEPT_ASSERT((Computation<
    computation::union_<always_disabled<1>, always_disabled<2>, always_enabled>
>));

BOOST_CONCEPT_ASSERT((Computation<
    computation::union_<
        always_disabled<1>, always_disabled<2>,
        always_disabled<3>, always_enabled
    >
>));


int main() { }
