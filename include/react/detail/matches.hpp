/*!
 * @file
 * This file defines `react::detail::matches`.
 */

#ifndef REACT_DETAIL_MATCHES_HPP
#define REACT_DETAIL_MATCHES_HPP

#include <react/detail/all_of.hpp>
#include <react/detail/any_of.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/type_traits/is_same.hpp>


namespace react { namespace detail {
namespace matches_detail {
using namespace boost;
using namespace boost::mpl;

//! By default, a type matches a pattern if they are the same.
//! Note: specializing matches<T, T> makes it ambiguous with the Node<...>
//!       specialization when Children1 is the same as Children2.
template <typename T, typename Pattern>
struct matches
    : is_same<T, Pattern>
{ };

//! Two template classes match if their template parameters match side by side.
template <template <typename ...> class Node,
          typename ...Children1,
          typename ...Children2>
struct matches<Node<Children1...>, Node<Children2...>>
    : and_<
        bool_<sizeof...(Children1) == sizeof...(Children2)>,
        all_of<
            zip_view<
                vector<
                    vector<Children1...>,
                    vector<Children2...>
                >
            >,
            unpack_args<matches<_1, _2>>
        >
    >
{ };

//! Matches anything.
struct match_any;
template <typename T>
struct matches<T, match_any> : true_ { };

//! Matches if any of the patterns match.
template <typename ...Patterns> struct match_any_of;
template <typename T, typename ...Patterns>
struct matches<T, match_any_of<Patterns...>>
    : any_of<
        vector<Patterns...>,
        matches<T, _1>
    >
{ };

//! Matches if all of the patterns match.
template <typename ...Patterns> struct match_all_of;
template <typename T, typename ...Patterns>
struct matches<T, match_all_of<Patterns...>>
    : all_of<
        vector<Patterns...>,
        matches<T, _1>
    >
{ };

//! Matches iff the pattern does not match.
template <typename Pattern> struct dont_match;
template <typename T, typename Pattern>
struct matches<T, dont_match<Pattern>>
    : not_<matches<T, Pattern>>
{ };

template <typename ...Patterns>
struct matches_any_of {
    template <typename T>
    struct apply
        : any_of<
            vector<Patterns...>,
            matches<T, _1>
        >
    { };
};

//! Matches if all of the children of the node matches any of the patterns.
template <template <typename ...> class Node,
          typename ...Children,
          typename ...Patterns>
struct matches<Node<Children...>, Node<match_all_of<Patterns...>>>
    : all_of<
        vector<Children...>,
        matches_detail::matches_any_of<Patterns...>
    >
{ };

template <typename FuncDecl> struct assert_matches_impl;

template <typename Ret, typename T, typename Pattern>
struct assert_matches_impl<Ret (T, Pattern)>
    : matches<T, Pattern>
{ };
} // end namespace matches_detail

using matches_detail::match_all_of;
using matches_detail::match_any;
using matches_detail::match_any_of;
using matches_detail::matches;

#define REACT_ASSERT_MATCHES(FuncParams)                                    \
    static_assert(                                                          \
        ::react::detail::matches_detail::assert_matches_impl<               \
            void FuncParams                                                 \
        >::value,                                                           \
        #FuncParams                                                         \
    )                                                                       \
/**/

#define REACT_ASSERT_MATCHES_NOT(FuncParams)                                \
    static_assert(                                                          \
      ! ::react::detail::matches_detail::assert_matches_impl<               \
        void FuncParams                                                     \
        >::value,                                                           \
        #FuncParams                                                         \
    )                                                                       \
/**/
}} // end namespace react::detail

#endif // !REACT_DETAIL_MATCHES_HPP
