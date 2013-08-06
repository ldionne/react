/*!
 * @file
 * This file defines the `react::name_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_NAME_OF_HPP
#define REACT_INTRINSIC_NAME_OF_HPP

#include <react/tag_of.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>


namespace react {
namespace name_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

    template <typename Computation>
    struct nested_name {
        using type = typename Computation::name;
    };

    template <typename Computation>
    struct anonymous_name {
        struct type;
    };
} // end namespace name_of_detail

namespace extension {
    template <typename Tag, typename Enable = void>
    struct name_of_impl {
        template <typename Computation>
        struct apply
            : boost::mpl::eval_if<
                name_of_detail::has_name<Computation>,
                name_of_detail::nested_name<Computation>,
                name_of_detail::anonymous_name<Computation>
            >
        { };
    };
} // end namespace extension

template <typename Computation>
struct name_of
    : extension::name_of_impl<
        typename tag_of<Computation>::type
    >::template apply<Computation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_NAME_OF_HPP
