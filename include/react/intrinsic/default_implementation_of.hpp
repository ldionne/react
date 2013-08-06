/*!
 * @file
 * This file defines the `react::default_implementation_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP
#define REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP

#include <react/traits.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>


namespace react {
namespace default_implementation_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(default_implementation)

    template <typename ComputationName>
    struct nested_default_implementation {
        using type = typename ComputationName::default_implementation;
    };

    template <typename ComputationName>
    struct default_impl
        : boost::lazy_enable_if<
            has_default_implementation<ComputationName>,
            nested_default_implementation<ComputationName>
        >
    { };
} // end namespace default_implementation_of_detail

namespace extension {
    template <typename Tag, typename Enable = void>
    struct default_implementation_of {
        template <typename ComputationName>
        struct apply
            : default_implementation_of_detail::default_impl<ComputationName>
        { };
    };
} // end namespace extension

template <typename ComputationName>
struct default_implementation_of
    : extension::default_implementation_of<
        typename tag_of<ComputationName>::type
    >::template apply<ComputationName>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP
