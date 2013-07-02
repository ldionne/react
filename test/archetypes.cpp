/*!
 * @file
 * This file contains compile-time unit tests for the archetypes.
 */

#include <react/archetypes.hpp>
#include <react/concepts.hpp>
#include <react/detail/empty_arg_pack.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>


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
    boost::mpl::vector<boost::null_archetype<>>, // valid semantic tags
    boost::mpl::map<>                            // dependencies
>;
BOOST_CONCEPT_ASSERT((check_incremental<detail::empty_arg_pack_type>));
BOOST_CONCEPT_ASSERT((check_incremental<default_construct>));


// FeatureSet
template <typename ConstructionArgs>
using check_feature_set = FeatureSet<
    feature_set_archetype<>,
    ConstructionArgs,
    boost::mpl::vector<boost::null_archetype<>>, // valid semantic tags
    boost::mpl::vector<boost::null_archetype<>>  // accessible features
>;
BOOST_CONCEPT_ASSERT((check_feature_set<detail::empty_arg_pack_type>));
BOOST_CONCEPT_ASSERT((check_feature_set<default_construct>));


int main() { }
