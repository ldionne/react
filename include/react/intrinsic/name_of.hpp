/*!
 * @file
 * This file defines the `react::name_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_NAME_OF_HPP
#define REACT_INTRINSIC_NAME_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>


namespace react {
namespace name_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

    template <typename Named>
    struct nested_name {
        using type = typename Named::name;
    };

    template <typename Named>
    struct anonymous_name {
        struct type;
    };
} // end namespace name_of_detail

namespace extension {
    template <typename RawNamed, typename Enable = void>
    struct name_of_impl {
        template <typename>
        struct apply
            : boost::mpl::eval_if<
                name_of_detail::has_name<RawNamed>,
                name_of_detail::nested_name<RawNamed>,
                name_of_detail::anonymous_name<RawNamed>
            >
        { };
    };
} // end namespace extension

template <typename Named>
struct name_of
    : extension::name_of_impl<
        typename detail::strip<Named>::type
    >::template apply<Named>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_NAME_OF_HPP
