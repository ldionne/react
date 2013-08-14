/*!
 * @file
 * This file contains unit tests for `react::computation::named`.
 */

#include <react/computation/named.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>
#include <react/intrinsic/name_of.hpp>

#include <boost/concept/assert.hpp>
#include <boost/type_traits/is_same.hpp>


using namespace react;

using NamedWrapper = computation::named<
    computation_name_archetype<>, computation_archetype<>
>;

struct NamedBase
    : computation::named<computation_name_archetype<>>
{ };

using NamedStandalone = computation::named<computation_name_archetype<>>;

BOOST_CONCEPT_ASSERT((Computation<NamedWrapper>));
static_assert(boost::is_same<
    name_of<NamedWrapper>::type, computation_name_archetype<>
>::value, "");

BOOST_CONCEPT_ASSERT((Computation<NamedBase>));
static_assert(boost::is_same<
    name_of<NamedBase>::type, computation_name_archetype<>
>::value, "");

BOOST_CONCEPT_ASSERT((Computation<NamedStandalone>));
static_assert(boost::is_same<
    name_of<NamedStandalone>::type, computation_name_archetype<>
>::value, "");


int main() { }
