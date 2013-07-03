/*!
 * @file
 * This file defines `react::factories::typed`.
 */

#ifndef REACT_FACTORIES_TYPED_HPP
#define REACT_FACTORIES_TYPED_HPP

#include <boost/type_traits/is_convertible.hpp>
#include <utility>


namespace react { namespace factories {
namespace typed_detail {
    template <typename T, typename Computation>
    struct typed_computation : Computation {
        using Computation::Computation;
        using Computation::operator=;

        using result_type = T;

        // note: we still use auto to allow proxy types, references and so on.
        template <typename FeatureSet>
        auto result(FeatureSet&& env) -> decltype(
            std::declval<Computation>().result(std::forward<FeatureSet>(env))
        ) {
            using UnderlyingResult = decltype(
                std::declval<Computation>().result(
                    std::forward<FeatureSet>(env))
            );
            static_assert(boost::is_convertible<
                UnderlyingResult, result_type
            >::value,
            "the result type of the underlying computation is not compatible "
            "with the type that is being enforced with `typed`");
            return Computation::result(std::forward<FeatureSet>(env));
        }
    };
} // end namespace typed_detail

/*!
 * Metafunction returning an incremental computation with a fixed result type.
 *
 * Computations created with this factory are equivalent to the original
 * computation, except that they have a nested type named `result_type`
 * representing a type to which the result of calling `result()` is
 * convertible.
 */
template <typename T, typename Computation>
struct typed {
    using type = typed_detail::typed_computation<T, Computation>;
};
}} // end namespace react::factories

#endif // !REACT_FACTORIES_TYPED_HPP
