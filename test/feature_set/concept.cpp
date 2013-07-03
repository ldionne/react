/*!
 * @file
 * This file contains concept checks for `react::feature_set`.
 */

#include <react/feature_set.hpp>
#include <react/concepts.hpp>
#include <react/detail/empty_arg_pack.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_set<>,
    default_construct,
    semantic_tags<>,
    dependency_results<>
>));

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_set<>,
    detail::empty_arg_pack_type,
    semantic_tags<>,
    dependency_results<>
>));


int main() { }