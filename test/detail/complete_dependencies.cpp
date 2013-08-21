/*!
 * @file
 * This file contains unit tests for `react::detail::complete_dependencies`.
 */

#include <react/detail/complete_dependencies.hpp>
#include <react/computation/depends_on.hpp>
#include <react/computation/implements.hpp>

#include <boost/mpl/set.hpp>
#include <boost/mpl/set_equal.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/vector.hpp>


using namespace react;
namespace mpl = boost::mpl;

template <typename ...Computations>
using completed = typename mpl::set_insert_range<
    mpl::set<>,
    typename detail::complete_dependencies<
        typename mpl::vector<Computations...>::type
    >::type
>::type;

using computation::depends_on;
using computation::implements;

namespace with_redundancy {
    struct c1 : implements<c1>, depends_on<> {
        using default_implementation = c1;
    };

    struct c2 : implements<c2>, depends_on<c1> {
        using default_implementation = c2;
    };

    struct c3 : implements<c3>, depends_on<c1> {
        using default_implementation = c3;
    };

    static_assert(mpl::set_equal<
        completed<c1>, mpl::set<c1>
    >::value, "");

    static_assert(mpl::set_equal<
        completed<c1, c2>, mpl::set<c1, c2>
    >::value, "");

    static_assert(mpl::set_equal<
        completed<c1, c2, c3>, mpl::set<c1, c2, c3>
    >::value, "");

    static_assert(mpl::set_equal<
        completed<c2>, mpl::set<c1, c2>
    >::value, "");

    static_assert(mpl::set_equal<
        completed<c3>, mpl::set<c1, c3>
    >::value, "");
} // end namespace with_redundancy

namespace with_linear_dependency_chain {
    struct c1 : implements<c1>, depends_on<> {
        using default_implementation = c1;
    };

    struct c2 : implements<c2>, depends_on<c1> {
        using default_implementation = c2;
    };

    struct c3 : implements<c3>, depends_on<c2> {

    };

    static_assert(mpl::set_equal<
        completed<c3>, mpl::set<c1, c2, c3>
    >::value, "");
} // end namespace with_linear_dependency_chain

namespace with_cyclic_dependency {
    struct c3;
    struct c1 : implements<c1>, depends_on<c3> {
        using default_implementation = c1;
    };

    struct c2 : implements<c2>, depends_on<c1> {
        using default_implementation = c2;
    };

    struct c3 : implements<c3>, depends_on<c2> {

    };

    static_assert(mpl::set_equal<
        completed<c3>, mpl::set<c1, c2, c3>
    >::value, "");
} // end namespace with_cyclic_dependency


int main() { }
