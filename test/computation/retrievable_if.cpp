/*!
 * @file
 * This file contains unit tests for `react::computation::retrievable_if`.
 */

#include <react/computation/retrievable_if.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation::retrievable_if<
        boost::mpl::always<boost::mpl::true_>,
        computation_archetype<>
    >
>));


int main() { }
