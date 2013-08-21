/*!
 * @file
 * This file contains unit tests for `react::computation::strict`.
 */

#include <react/computation/strict.hpp>
#include <react/archetypes.hpp>
#include <react/computation/depends_on.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

template <int>
struct dependency : feature_archetype<> { };

BOOST_CONCEPT_ASSERT((Computation<
    computation::strict<computation_archetype<>>
>));

BOOST_CONCEPT_ASSERT((Computation<
    computation::strict<computation::depends_on<dependency<1>>>,
    fake_result_of<dependency<1>>::with<boost::copy_constructible_archetype<>>
>));

BOOST_CONCEPT_ASSERT((Computation<
    computation::strict<computation::depends_on<dependency<1>, dependency<2>>>,
    fake_result_of<dependency<1>>::with<boost::copy_constructible_archetype<>>,
    fake_result_of<dependency<2>>::with<boost::copy_constructible_archetype<>>
>));


int main() { }
