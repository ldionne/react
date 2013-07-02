/*!
 * @file
 * This file defines `react::detail::all_of`.
 */

#ifndef REACT_DETAIL_ALL_OF_HPP
#define REACT_DETAIL_ALL_OF_HPP

#include <react/detail/none_of.hpp>

#include <boost/mpl/bind.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>


namespace react { namespace detail {
namespace all_of_detail {
    using namespace boost::mpl;
    template <typename Sequence, typename Pred>
    struct all_of
        : none_of<
            Sequence,
            bind<
                typename lambda<not_<_1> >::type,
                bind<typename lambda<Pred>::type, _1>
            >
        >
    { };
}
using all_of_detail::all_of;
}} // end namespace boost::mpl

#endif // !REACT_DETAIL_ALL_OF_HPP
