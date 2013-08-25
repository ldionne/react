/*!
 * @file
 * This file contains unit tests for `react::detail::flatten_into_set`.
 */

#include <react/detail/flatten_into_set.hpp>

#include <boost/mpl/set.hpp>
#include <boost/mpl/set_equal.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/vector.hpp>


using namespace boost::mpl;
using react::detail::flatten_into_set;

template <int> struct T;

namespace with_nothing_to_flatten {
    template <typename ...T>
    using test = set_equal<
        typename flatten_into_set<typename vector<T...>::type>::type,
        typename set_insert_range<
            typename set<>::type,
            typename vector<T...>::type
        >::type
    >;

    static_assert(test<>::value, "");
    static_assert(test<T<0>>::value, "");
    static_assert(test<T<0>, T<1>>::value, "");
    static_assert(test<T<0>, T<1>, T<2>>::value, "");
    static_assert(test<T<0>, T<1>, T<2>, T<2>>::value, "");
} // end namespace with_nothing_to_flatten

namespace with_stuff_to_flatten {
    static_assert(set_equal<
        flatten_into_set<vector<T<0>, vector<T<1>, T<2>>>>::type,
        set<T<0>, T<1>, T<2>>::type
    >::value, "");
} // end namespace with_stuff_to_flatten


int main() { }
