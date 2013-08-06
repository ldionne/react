/*!
 * @file
 * This file contains unit tests for `react::detail::bind`.
 */

#include <react/detail/bind.hpp>
#include <react/computation/depends_on.hpp>
#include <react/computation/named.hpp>
#include <react/intrinsic/name_of.hpp>
#include <react/placeholders.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>


using namespace react;
namespace mpl = boost::mpl;

using computation::named;
using computation::depends_on;

template <typename ...Computations>
using bound = typename mpl::make_index_of<
    typename detail::bind<
        typename mpl::vector<Computations...>::type
    >::type,
    name_of<mpl::_1>
>::type;

namespace scenario1 {
    struct c1 : named<c1> { };
    struct c2 : named<c2> { };

    template <typename Deps = placeholders::dependencies>
    struct sub : named<sub<>>, depends_on<c1, c2> {
        using Dependencies = Deps;
    };

    using BoundMap = bound<c1, c2, sub<>>;
    using Dependencies = mpl::at<BoundMap, sub<>>::type::Dependencies;

    static_assert(boost::is_same<
        mpl::at<Dependencies, c1>::type, c1
    >::value, "");
    static_assert(boost::is_same<
        mpl::at<Dependencies, c2>::type, c2
    >::value, "");
}


int main() { }
