/*!
 * @file
 * This file defines unit tests for `react::feature_sets::by_ref`.
 */

#include <react/feature_sets/by_ref.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::by_ref<feature_set_archetype<>>,
    semantic_tags<boost::null_archetype<>>,
    dependency_results<>
>));


int main() { }
