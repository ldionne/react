/*!
 * @file
 * This file contains unit tests for `react::computation::implementing`.
 */

#include <react/computation/implementing.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/concept/assert.hpp>
#include <boost/type_traits/is_same.hpp>


using namespace react;

using Wrapper = computation::implementing<
    feature_archetype<>, computation_archetype<>
>;

struct Base
    : computation::implementing<feature_archetype<>>
{ };

using Standalone = computation::implementing<feature_archetype<>>;

BOOST_CONCEPT_ASSERT((Computation<Wrapper>));
static_assert(boost::is_same<
    feature_of<Wrapper>::type, feature_archetype<>
>::value, "");

BOOST_CONCEPT_ASSERT((Computation<Base>));
static_assert(boost::is_same<
    feature_of<Base>::type, feature_archetype<>
>::value, "");

BOOST_CONCEPT_ASSERT((Computation<Standalone>));
static_assert(boost::is_same<
    feature_of<Standalone>::type, feature_archetype<>
>::value, "");


int main() { }
