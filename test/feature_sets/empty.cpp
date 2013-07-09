/*!
 * @file
 * This file contains concept checks for `react::feature_sets::empty`.
 */

#include <react/feature_sets/empty.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::empty,
    semantic_tags<>,
    dependency_results<>
>));


int main() { }
