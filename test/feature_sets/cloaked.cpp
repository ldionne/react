/*!
 * @file
 * This file defines unit tests for `react::feature_sets::cloaked`.
 */

#include <react/feature_sets/cloaked.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>
#include <utility>


namespace mpl = boost::mpl;
using namespace react;

template <typename ...Dependencies>
struct computation_depending_on : incremental_computation_archetype<> {
    using dependencies = mpl::set<Dependencies...>;
};

template <typename ...Dependencies>
using access_only = feature_sets::cloaked<
    feature_set_archetype<>, computation_depending_on<Dependencies...>
>;

struct feature1;
struct feature2;
struct feature3;
struct feature4;

namespace concept {
    template <typename ...Dependencies>
    using test_concept = FeatureSet<
        access_only<Dependencies...>,
        semantic_tags<boost::null_archetype<>>,
        accessible_features<Dependencies...>
    >;

    BOOST_CONCEPT_ASSERT((test_concept<>));
    BOOST_CONCEPT_ASSERT((test_concept<feature1>));
    BOOST_CONCEPT_ASSERT((test_concept<feature1, feature2>));
    BOOST_CONCEPT_ASSERT((test_concept<feature1, feature2, feature3>));
}

namespace accessibility {
    template <typename FeatureSet, typename Feature, typename = decltype(
        std::declval<FeatureSet&>()[std::declval<Feature const&>()]
    )> constexpr bool is_accessible(void*) { return true; }

    template <typename FeatureSet, typename Feature>
    constexpr bool is_accessible(...) { return false; }

    // The specified feature should be accessible, but not other unrelated
    // features.
    static_assert(is_accessible<access_only<feature1>, feature1>(0), "");
    static_assert(!is_accessible<access_only<feature1>, feature2>(0), "");
    static_assert(!is_accessible<access_only<feature1>, feature3>(0), "");
    static_assert(!is_accessible<access_only<feature1>, feature4>(0), "");


    // The implicitly added dependencies should not be accessible.
    static_assert(is_accessible<access_only<feature2>, feature2>(0), "");
    static_assert(!is_accessible<access_only<feature2>, feature1>(0), "");


    // Same as before, but with different sets.
    static_assert(is_accessible<access_only<feature3>, feature3>(0), "");
    static_assert(!is_accessible<access_only<feature3>, feature1>(0), "");
    static_assert(!is_accessible<access_only<feature3>, feature2>(0), "");

    static_assert(is_accessible<access_only<feature4>, feature4>(0), "");
    static_assert(!is_accessible<access_only<feature4>, feature1>(0), "");
    static_assert(!is_accessible<access_only<feature4>, feature2>(0), "");
    static_assert(!is_accessible<access_only<feature4>, feature3>(0), "");

    static_assert(is_accessible<access_only<feature1, feature3>, feature1>(0), "");
    static_assert(is_accessible<access_only<feature1, feature3>, feature3>(0), "");
    static_assert(!is_accessible<access_only<feature1, feature3>, feature2>(0), "");

    using access_any = access_only<feature1, feature2, feature3, feature4>;
    static_assert(is_accessible<access_any, feature1>(0), "");
    static_assert(is_accessible<access_any, feature2>(0), "");
    static_assert(is_accessible<access_any, feature3>(0), "");
    static_assert(is_accessible<access_any, feature4>(0), "");
}


int main() { }
