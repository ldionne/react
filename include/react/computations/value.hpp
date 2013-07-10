/*!
 * @file
 * This file defines `react::computations::value`.
 */

#ifndef REACT_COMPUTATIONS_VALUE_HPP
#define REACT_COMPUTATIONS_VALUE_HPP

#include <react/computations/typed.hpp>
#include <react/depends_on.hpp>
#include <react/detail/dont_care.hpp>
#include <react/implements.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <utility>


namespace react { namespace computations {
namespace value_detail {
    template <typename T, typename Feature, typename InitializationFeature>
    struct value_impl : implements<Feature>, depends_on<InitializationFeature>
    {
        value_impl() = default;

        template <typename FeatureSet>
        explicit value_impl(FeatureSet&& fset)
            : value_(std::forward<FeatureSet>(fset)[InitializationFeature{}])
        { }

        T& result(detail::dont_care) { return value_; }
        T const& result(detail::dont_care) const { return value_; }

    private:
        T value_;
    };

    template <typename T, typename Feature, typename InitializationFeature>
    struct make_value_computation {
        using AdjustedType = typename boost::mpl::eval_if<
            boost::is_function<T>,
            boost::add_pointer<T>,
            boost::mpl::identity<T>
        >::type;

        using type = typed<
            AdjustedType, value_impl<
                AdjustedType, Feature, InitializationFeature
            >
        >;
    };
} // end namespace value_detail

/*!
 * Computation implemented as a single value.
 *
 * When constructing the computation, its initial value must be provided in
 * a `FeatureSet` implementing an `InitializationFeature`. Otherwise, if the
 * computation is default-constructed, the value stored in the computation is
 * default-constructed.
 *
 * A value computation can't be executed. When its result is retrieved,
 * it returns a reference to the value it holds internally, with a `const`
 * qualifier matching that of the computation.
 *
 * If a function type is specified, a pointer to the function type is
 * stored instead. Storing array types is not supported; use `std::array`
 * instead.
 *
 * Computations created with this factory are implicitly `typed`.
 */
template <typename T, typename Feature, typename InitializationFeature>
using value = typename value_detail::make_value_computation<
    T, Feature, InitializationFeature
>::type;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_VALUE_HPP
