/*!
 * @file
 * This file contains unit tests for `react::detail::dependency_graph`.
 */

#include <react/detail/dependency_graph.hpp>
#include <react/archetypes.hpp>
#include <react/computation/depends_on.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_equal.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/vector.hpp>


using namespace react;
namespace mpl = boost::mpl;

template <typename ...Computations>
using graph = detail::dependency_graph<
    typename mpl::vector<Computations...>::type
>;

template <typename Graph>
using computations_in = typename mpl::set_insert_range<
    typename mpl::set<>::type,
    typename mpl::vertices_of<Graph>::type
>::type;

static_assert(mpl::set_equal<
    computations_in<graph<>>,
    mpl::set<>
>::value, "");

static_assert(mpl::set_equal<
    computations_in<graph<computation_archetype<>>>,
    mpl::set<computation_archetype<>>
>::value, "");

// Duplicate computations do not create duplicate vertices.
static_assert(mpl::set_equal<
    computations_in<graph<computation_archetype<>, computation_archetype<>>>,
    mpl::set<computation_archetype<>>
>::value, "");

// Dependencies are handled by name.
static_assert(mpl::set_equal<
    computations_in<graph<
        computation::depends_on<feature_of<computation_archetype<>>::type>,
        computation_archetype<>
    >>,
    mpl::set<
        computation::depends_on<feature_of<computation_archetype<>>::type>,
        computation_archetype<>
    >
>::value, "");

// Duplicate dependencies should not wreak havoc.
static_assert(mpl::set_equal<
    computations_in<graph<
        computation::depends_on<
            feature_of<computation_archetype<>>::type,
            feature_of<computation_archetype<>>::type
        >,
        computation_archetype<>
    >>,
    mpl::set<
        computation::depends_on<
            feature_of<computation_archetype<>>::type,
            feature_of<computation_archetype<>>::type
        >,
        computation_archetype<>
    >
>::value, "");

// Missing dependencies should not appear in the graph.
static_assert(mpl::set_equal<
    computations_in<graph<
        computation::depends_on<struct some_name>
    >>,
    mpl::set<
        computation::depends_on<struct some_name>
    >
>::value, "");


int main() { }
