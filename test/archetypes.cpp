/*!
 * @file
 * This file contains compile-time unit tests for the archetypes.
 */

#include <react/archetypes.hpp>
#include <react/concepts.hpp>
#include <react/detail/empty_arg_pack.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

// CompositeFeature
BOOST_CONCEPT_ASSERT((CompositeFeature<composite_feature_archetype<>>));


// Feature
BOOST_CONCEPT_ASSERT((Feature<feature_archetype<>>));


// IncrementalComputation
template <typename ConstructionArgs>
using check_incremental = IncrementalComputation<
    incremental_computation_archetype<>,
    ConstructionArgs,
    semantic_tags<boost::null_archetype<>>,
    dependency_results<>
>;
BOOST_CONCEPT_ASSERT((check_incremental<detail::empty_arg_pack_type>));
BOOST_CONCEPT_ASSERT((check_incremental<default_construct>));


// FeatureSet
BOOST_CONCEPT_ASSERT((FeatureSet<
    feature_set_archetype<>,
    semantic_tags<boost::null_archetype<>>,
    accessible_features<boost::null_archetype<>>
>));


int main() { }
