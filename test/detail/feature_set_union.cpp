/*!
 * @file
 * This file defines unit tests for `react::detail::feature_set_union`.
 */

#include <react/detail/feature_set_union.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <utility>


using namespace react;

using feature_set_union = decltype(
    detail::feature_set_union(
        std::declval<feature_set_archetype<>&>(),
        std::declval<feature_set_archetype<>&>()
    )
);
struct default_constructible : feature_set_union {
    default_constructible();
};

BOOST_CONCEPT_ASSERT((FeatureSet<
    default_constructible,
    default_construct,
    semantic_tags<>,
    accessible_features<>
>));


int main() { }
