/*!
 * @file
 * This file defines `react::detail::feature_set_union`.
 */

#ifndef REACT_DETAIL_FEATURE_SET_UNION_HPP
#define REACT_DETAIL_FEATURE_SET_UNION_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <boost/mpl/set_union.hpp>
#include <utility>


namespace react { namespace detail {
namespace feature_set_union_detail {
    template <typename FS1, typename FS2>
    class feature_set_union_type;
}

template <typename FS1, typename FS2>
feature_set_union_detail::feature_set_union_type<FS1, FS2>
feature_set_union(FS1& fs1, FS2& fs2) {
    return {fs1, fs2};
}

namespace feature_set_union_detail {
    template <typename FS1, typename FS2>
    class feature_set_union_type {
        FS1& fs1_;
        FS2& fs2_;

        feature_set_union_type(FS1& fs1, FS2& fs2)
            : fs1_(fs1), fs2_(fs2)
        { }

        template <typename, typename>
        friend feature_set_union_type feature_set_union(FS1& fs1, FS2& fs2);

    public:
        using computations = typename boost::mpl::set_union<
            typename computations_of<FS1>::type,
            typename computations_of<FS2>::type
        >::type;

        template <typename SemanticTag>
        void operator()(SemanticTag const& tag) {
            fs1_(tag, fs2_);
            fs2_(tag, fs1_);
        }

        template <typename SemanticTag, typename FeatureSet>
        auto operator()(SemanticTag const& tag, FeatureSet&& ext)
        REACT_AUTO_RETURN(
            feature_set_union(*this, std::forward<FeatureSet>(ext))(tag)
        )

        template <typename Feature>
        auto operator[](Feature&& feature) REACT_AUTO_RETURN(
            fs1_[std::forward<Feature>(feature)]
        )

        template <typename Feature>
        auto operator[](Feature&& feature) REACT_AUTO_RETURN(
            fs2_[std::forward<Feature>(feature)]
        )
    };
} // end namespace feature_set_union_detail
}} // end namespace react::detail

#endif // !REACT_DETAIL_FEATURE_SET_UNION_HPP
