/*!
 * @file
 * This file contains unit tests for `react::detail::topological_indexing`.
 */

#include <react/detail/topological_indexing.hpp>
#include <react/archetypes.hpp>
#include <react/computation/depends_on.hpp>
#include <react/traits.hpp>

#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/vector.hpp>


namespace mpl = boost::mpl;
using namespace react;

template <typename ...Computations>
using indexing_of = typename mpl::copy<
    typename detail::topological_indexing<mpl::vector<Computations...>>::type,
    mpl::back_inserter<mpl::vector<>>
>::type;

// `mpl::vector_c<long>` creates a `mpl::vector<>` of `mpl::integral_c<long>`
// instead of a `mpl::vector` of `mpl::long_`s, hence this workaround.
template <long ...Indices>
using indices = typename mpl::vector<mpl::long_<Indices>...>::type;

static_assert(mpl::equal<
    indexing_of<>,
    indices<>
>::value, "");

static_assert(mpl::equal<
    indexing_of<computation_archetype<>>,
    indices<0>
>::value, "");

// Duplicate computations should be ignored.
static_assert(mpl::equal<
    indexing_of<computation_archetype<>, computation_archetype<>>,
    indices<0>
>::value, "");

namespace test_ordering_with_3_linear_computations {
    struct a { };
    struct b : computation::depends_on<name_of<a>::type> { };
    struct c : computation::depends_on<name_of<b>::type> { };

    static_assert(mpl::equal<
        indexing_of<a, b, c>, indices<0, 1, 2>
    >::value, "");

    static_assert(mpl::equal<
        indexing_of<c, b, a>, indices<2, 1, 0>
    >::value, "");

    static_assert(mpl::equal<
        indexing_of<b, c, a>, indices<2, 0, 1>
    >::value, "");
}


int main() { }
