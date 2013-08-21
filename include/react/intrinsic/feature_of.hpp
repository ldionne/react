/*!
 * @file
 * This file defines the `react::feature_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_FEATURE_OF_HPP
#define REACT_INTRINSIC_FEATURE_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>


namespace react {
namespace feature_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(feature)

    template <typename Implementation>
    struct nested_feature {
        using type = typename Implementation::feature;
    };

    template <typename Implementation>
    struct anonymous_feature {
        struct type;
    };
} // end namespace feature_of_detail

namespace extension {
    template <typename RawImplementation, typename Enable = void>
    struct feature_of_impl {
        template <typename>
        struct apply
            : boost::mpl::eval_if<
                feature_of_detail::has_feature<RawImplementation>,
                feature_of_detail::nested_feature<RawImplementation>,
                feature_of_detail::anonymous_feature<RawImplementation>
            >
        { };
    };
} // end namespace extension

template <typename Implementation>
struct feature_of
    : extension::feature_of_impl<
        typename detail::strip<Implementation>::type
    >::template apply<Implementation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_FEATURE_OF_HPP
