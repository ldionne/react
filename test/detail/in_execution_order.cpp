/*!
 * @file
 * This file contains unit tests for `react::detail::in_execution_order`.
 */

#include <react/detail/in_execution_order.hpp>
#include <react/computation/executed_after.hpp>
#include <react/computation/executed_before.hpp>
#include <react/computation/implementing.hpp>

#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>


namespace mpl = boost::mpl;
using react::detail::in_execution_order;
using namespace react::computation;

template <typename ...T>
using vector = typename mpl::vector<T...>::type;

namespace scenario1 {
    struct c1; struct c2; struct c3;
    struct c1 : implementing<c1> { };
    struct c2 : implementing<c2>, executed_after<c1>, executed_before<c3> { };
    struct c3 : implementing<c3> { };

    static_assert(mpl::equal<
        in_execution_order<vector<c1, c2, c3>>::type,
        vector<c1, c2, c3>
    >::value, "");

    static_assert(mpl::equal<
        in_execution_order<vector<c3, c2, c1>>::type,
        vector<c1, c2, c3>
    >::value, "");

    static_assert(mpl::equal<
        in_execution_order<vector<c2, c3, c1>>::type,
        vector<c1, c2, c3>
    >::value, "");
} // end namespace scenario1


int main() { }
