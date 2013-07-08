/*!
 * @file
 * This file defines unit tests for `react::feature_sets::union_`.
 */

#include <react/feature_sets/union.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::union_<feature_set_archetype<>, feature_set_archetype<>>,
    semantic_tags<>,
    accessible_features<>
>));


int main() { }
