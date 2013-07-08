/*!
 * @file
 * This file defines unit tests for `react::feature_sets::from_argument_pack`.
 */

#include <react/feature_sets/from_argument_pack.hpp>
#include <react/concepts.hpp>
#include <react/detail/empty_arg_pack.hpp>

#include <boost/concept/assert.hpp>
#include <boost/parameter/keyword.hpp>


using namespace react;
using boost::parameter::keyword;

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::from_argument_pack<detail::empty_arg_pack_type>,
    semantic_tags<>,
    accessible_features<>
>));

struct feature1 { };
struct feature2 { };

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::from_argument_pack<decltype((
        keyword<feature1>::get() = "anything"
    ))>,
    semantic_tags<>,
    accessible_features<feature1>
>));

BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_sets::from_argument_pack<decltype((
        keyword<feature1>::get() = "anything",
        keyword<feature2>::get() = "anything"
    ))>,
    semantic_tags<>,
    accessible_features<feature1, feature2>
>));


int main() { }
