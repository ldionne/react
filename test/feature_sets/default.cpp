/*!
 * @file
 * This file contains concept checks for `react::feature_sets::default_`.
 */

#include <react/feature_sets/default.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::default_<>,
    semantic_tags<>,
    dependency_results<>
>));


int main() { }
