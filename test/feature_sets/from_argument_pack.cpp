/*!
 * @file
 * This file defines unit tests for `react::feature_sets::from_argument_pack`.
 */

#include <react/feature_sets/from_argument_pack.hpp>
#include <react/concepts.hpp>
#include <react/detail/empty_arg_pack.hpp>

#include <boost/concept/assert.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::from_argument_pack<detail::empty_arg_pack_type>,
    semantic_tags<>,
    accessible_features<>
>));


int main() { }
