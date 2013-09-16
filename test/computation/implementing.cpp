/*!
 * @file
 * Contains unit tests for `react::computation::implementing`.
 */

#include <react/computation/implementing.hpp>

#include <react/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <type_traits>


using namespace react;

using Wrapper = computation::implementing<
    feature_archetype<>, computation_archetype<>
>;

struct Base
    : computation::implementing<feature_archetype<>>
{ };

using Standalone = computation::implementing<feature_archetype<>>;

REACT_CONCEPT_ASSERT(Computation<Wrapper>);
static_assert(std::is_same<
    feature_of<Wrapper>::type, feature_archetype<>
>::value, "");

REACT_CONCEPT_ASSERT(Computation<Base>);
static_assert(std::is_same<
    feature_of<Base>::type, feature_archetype<>
>::value, "");

REACT_CONCEPT_ASSERT(Computation<Standalone>);
static_assert(std::is_same<
    feature_of<Standalone>::type, feature_archetype<>
>::value, "");


int main() { }
