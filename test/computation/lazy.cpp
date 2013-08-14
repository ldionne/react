/*!
 * @file
 * This file contains unit tests for `react::computation::lazy`.
 */

#include <react/computation/lazy.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation::lazy<computation_archetype<>>
>));


int main() { }
