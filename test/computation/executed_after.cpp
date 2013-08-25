/*!
 * @file
 * This file contains unit tests for `react::computation::executed_after`.
 */

#include <react/computation/executed_after.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation::executed_after<feature_archetype<>>
>));


int main() { }
