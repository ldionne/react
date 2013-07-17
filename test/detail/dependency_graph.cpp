/*!
 * @file
 * This file contains unit tests for `react::detail::dependency_graph`.
 */

#include <react/detail/dependency_graph.hpp>
#include <react/archetypes.hpp>
#include <react/computations/depends_on.hpp>
#include <react/traits.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_equal.hpp>
#include <boost/mpl/vector.hpp>


using namespace react;
namespace mpl = boost::mpl;

template <typename ...Computations>
using graph = detail::dependency_graph<
    typename mpl::vector<Computations...>::type
>;

template <typename Graph>
using computations_in = typename mpl::fold<
    typename mpl::vertices_of<Graph>::type,
    mpl::set<>,
    mpl::insert<mpl::_1, mpl::at<Graph, mpl::_2>>
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
        computations::depends_on<name_of<computation_archetype<>>::type>,
        computation_archetype<>
    >>,
    mpl::set<
        computations::depends_on<name_of<computation_archetype<>>::type>,
        computation_archetype<>
    >
>::value, "");

// Duplicate dependencies should not wreak havoc.
static_assert(mpl::set_equal<
    computations_in<graph<
        computations::depends_on<
            name_of<computation_archetype<>>::type,
            name_of<computation_archetype<>>::type
        >,
        computation_archetype<>
    >>,
    mpl::set<
        computations::depends_on<
            name_of<computation_archetype<>>::type,
            name_of<computation_archetype<>>::type
        >,
        computation_archetype<>
    >
>::value, "");


int main() { }
