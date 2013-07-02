/*!
 * @file
 * This file contains the definition of `react::detail::deferred_apply` and
 * `react::detail::deferred_apply_wrap`.
 */

#ifndef REACT_DETAIL_DEFERRED_APPLY_HPP
#define REACT_DETAIL_DEFERRED_APPLY_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/lambda.hpp>


namespace react { namespace detail {
namespace deferred_apply_detail {
    template <typename T, typename ...Args>
    struct fetch_template_apply
    { typedef typename T::template apply<Args...> type; };

    template <typename T>
    struct fetch_non_template_apply
    { typedef typename T::apply type; };

    struct dont_fetch_anything { };

    // For some reason, boost/mpl/has_xxx does not seem to work with
    // variadic templates, so we're rolling our own SFINAE.
    template <typename T, typename ...Args>
    struct pick_apply_fetcher {
        template <typename U> static
        fetch_template_apply<U, Args...>
        test(typename U::template apply<Args...>* =0);

        template <typename U> static
        fetch_non_template_apply<U>
        test(typename U::apply* =0);

        template <typename U> static
        dont_fetch_anything
        test(...);

        typedef decltype(test<T>(0)) type;
    };
} // end namespace deferred_apply_detail

template <typename MetafunctionClass, typename ...Args>
struct deferred_apply_wrap
    : deferred_apply_detail::pick_apply_fetcher<
        MetafunctionClass, Args...
    >::type
{ };

template <typename MetafunctionClass, typename ...Args>
struct deferred_apply
    : deferred_apply_wrap<
        typename boost::mpl::lambda<MetafunctionClass>::type, Args...
    >
{ };
}} // end namespace react::detail

#endif // !REACT_DETAIL_DEFERRED_APPLY_HPP
