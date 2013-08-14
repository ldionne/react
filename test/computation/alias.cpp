/*!
 * @file
 * This file contains unit tests for `react::computation::alias`.
 */

#include <react/computation/alias.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation::alias<computation_name_archetype<>>,
    fake_result_of<
        computation_name_archetype<>
    >::with<
        boost::copy_constructible_archetype<>
    >
>));


int main() { }
