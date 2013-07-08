/*!
 * @file
 * This file defines `react::feature_sets::union_`.
 */

#ifndef REACT_FEATURE_SETS_UNION_HPP
#define REACT_FEATURE_SETS_UNION_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <utility>


namespace react { namespace feature_sets {
template <typename FS1, typename FS2>
class union_ {
    FS1& fs1_;
    FS2& fs2_;

public:
    union_(FS1& fs1, FS2& fs2)
        : fs1_(fs1), fs2_(fs2)
    { }

    template <typename SemanticTag>
    void operator()(SemanticTag const& tag) {
        fs1_(tag, fs2_);
        fs2_(tag, fs1_);
    }

    template <typename SemanticTag, typename FeatureSet>
    auto operator()(SemanticTag const& tag, FeatureSet&& ext)
    REACT_AUTO_RETURN(
        union_(*this, std::forward<FeatureSet>(ext))(tag)
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

template <typename FS1, typename FS2>
union_<FS1, FS2> make_union(FS1& fs1, FS2& fs2) {
    return {fs1, fs2};
}
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_UNION_HPP
