/*!
 * @file
 * This file contains compile-time unit tests for the concept archetypes.
 */

#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<computation_archetype<>>));
BOOST_CONCEPT_ASSERT((Feature<feature_archetype<>>));
BOOST_CONCEPT_ASSERT((Environment<environment_archetype<>>));
BOOST_CONCEPT_ASSERT((Implementation<implementation_archetype<>>));


int main() { }
