/*!
 * @file
 * This file defines the `react::dependencies_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_DEPENDENCIES_OF_HPP
#define REACT_INTRINSIC_DEPENDENCIES_OF_HPP

#include <react/tag_of.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/set.hpp>


namespace react {
namespace dependencies_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(dependencies)

    template <typename Computation>
    struct nested_dependencies {
        using type = typename Computation::dependencies;
    };
} // end namespace dependencies_of_detail

namespace extension {
    template <typename Tag, typename Enable = void>
    struct dependencies_of_impl {
        template <typename Computation>
        struct apply
            : boost::mpl::eval_if<
                dependencies_of_detail::has_dependencies<Computation>,
                dependencies_of_detail::nested_dependencies<Computation>,
                boost::mpl::set<>
            >
        { };
    };
} // end namespace extension

template <typename Computation>
struct dependencies_of
    : extension::dependencies_of_impl<
        typename tag_of<Computation>::type
    >::template apply<Computation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_DEPENDENCIES_OF_HPP
