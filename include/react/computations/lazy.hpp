/*!
 * @file
 * This file defines `react::computations::lazy`.
 */

#ifndef REACT_COMPUTATIONS_LAZY_HPP
#define REACT_COMPUTATIONS_LAZY_HPP

#include <react/detail/auto_return.hpp>


namespace react { namespace computations {
namespace lazy_detail {
    template <typename Computation, typename FeatureSet>
    class call_result {
        Computation& computation_;
        FeatureSet& fset_;

    public:
        call_result(Computation& computation, FeatureSet& fset)
            : computation_(computation), fset_(fset)
        { }

        template <typename OtherFeatureSet>
        auto operator()(OtherFeatureSet& other_fset) REACT_AUTO_RETURN(
            computation_.result(other_fset)
        )

        auto operator()() REACT_AUTO_RETURN(
            computation_.result(fset_)
        )
    };
} // end namespace lazy_detail

template <typename Computation>
struct lazy : Computation {
    using Computation::Computation;
    using Computation::operator=;

    template <typename FeatureSet>
    lazy_detail::call_result<Computation, FeatureSet>
    result(FeatureSet& fset) {
        return {*this, fset};
    }
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_LAZY_HPP
