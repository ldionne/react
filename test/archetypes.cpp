/*!
 * @file
 * This file contains compile-time unit tests for the concept archetypes.
 */

#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation_archetype<>,
    dependency_results<>
>));

BOOST_CONCEPT_ASSERT((Environment<
    environment_archetype<>,
    implemented_computations<boost::null_archetype<>>
>));


int main() { }
