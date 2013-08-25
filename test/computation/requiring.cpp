/*!
 * @file
 * This file contains unit tests for `react::computation::requiring`.
 */

#include <react/computation/requiring.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

template <int>
struct requirement : feature_archetype<> { };

template <typename ...Requirements>
using test = Computation<
    computation::requiring<Requirements...>,
    typename fake_result_of<
        Requirements
    >::template with<
        boost::copy_constructible_archetype<>
    >...
>;

BOOST_CONCEPT_ASSERT((test<>));
BOOST_CONCEPT_ASSERT((test<dependency<1>>));
BOOST_CONCEPT_ASSERT((test<dependency<1>, dependency<2>>));


int main() { }
