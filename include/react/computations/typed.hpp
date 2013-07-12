/*!
 * @file
 * This file defines `react::computations::typed`.
 */

#ifndef REACT_COMPUTATIONS_TYPED_HPP
#define REACT_COMPUTATIONS_TYPED_HPP

#include <react/detail/auto_return.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <utility>


namespace react { namespace computations {
/*!
 * Computation with a fixed result type.
 *
 * `typed` computations are equivalent to the original computation, except
 * that they have a nested type named `result_type` representing a type to
 * which the result of calling `result()` is convertible.
 */
template <typename T, typename Computation>
struct typed : Computation {
    using Computation::Computation;
    using Computation::operator=;

    using result_type = T;

private:
    // note: we still use auto to allow proxy types, references and so on.
    template <typename Comp, typename FeatureSet>
    static auto result_impl(Comp&& comp, FeatureSet&& fset) -> decltype(
        std::forward<Comp>(comp).result(std::forward<FeatureSet>(fset))
    ) {
        using UnderlyingResult = decltype(
            std::forward<Comp>(comp).result(std::forward<FeatureSet>(fset))
        );
        static_assert(boost::is_convertible<
            UnderlyingResult, result_type
        >::value,
        "the result type of the underlying computation is not compatible "
        "with the type that is being enforced with `typed`");
        return std::forward<Comp>(comp).result(std::forward<FeatureSet>(fset));
    }

public:
    template <typename FeatureSet>
    auto result(FeatureSet&& fset) REACT_AUTO_RETURN(
        result_impl(
            static_cast<Computation&>(*this),
            std::forward<FeatureSet>(fset)
        )
    )

    template <typename FeatureSet>
    auto result(FeatureSet&& fset) const REACT_AUTO_RETURN(
        result_impl(
            static_cast<Computation const&>(*this),
            std::forward<FeatureSet>(fset)
        )
    )
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_TYPED_HPP
