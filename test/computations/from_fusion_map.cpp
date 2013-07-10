/*!
 * @file
 * This file contains unit tests for `react::computations::from_fusion_map`.
 */

#include <react/computations/from_fusion_map.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/mpl/pair.hpp>


using namespace react;
namespace fusion = boost::fusion;

struct map_feature : feature_archetype<> { };
struct key;
BOOST_CONCEPT_ASSERT((IncrementalComputation<
    computations::from_fusion_map<feature_archetype<>, map_feature, key>,
    default_construct,
    semantic_tags<>,
    dependency_results<
        boost::mpl::pair<
            map_feature,
            fusion::map<fusion::pair<key, boost::null_archetype<>&>>
        >
    >
>));


int main() { }
