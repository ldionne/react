/*!
 * @file
 * This file defines `react::computations::variable`.
 */

#ifndef REACT_COMPUTATIONS_VARIABLE_HPP
#define REACT_COMPUTATIONS_VARIABLE_HPP

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
namespace variable_detail {
    template <
        typename ImplementedFeature,
        typename Type,
        typename InitializationFeature
    >
    struct variable_impl
        : implements<ImplementedFeature>,
          depends_on<InitializationFeature>
    {
        variable_impl() = default;

        template <typename FeatureSet>
        explicit variable_impl(FeatureSet&& fset)
            : value_(std::forward<FeatureSet>(fset)[InitializationFeature{}])
        { }

        Type& result(detail::dont_care) { return value_; }
        Type const& result(detail::dont_care) const { return value_; }

    private:
        Type value_;
    };

    template <
        typename ImplementedFeature,
        typename Type,
        typename InitializationFeature
    >
    struct make_variable {
        using AdjustedType = typename boost::mpl::eval_if<
            boost::is_function<Type>,
            boost::add_pointer<Type>,
            boost::mpl::identity<Type>
        >::type;

        using type = typed<
            AdjustedType,
            variable_impl<
                ImplementedFeature, AdjustedType, InitializationFeature
            >
        >;
    };
} // end namespace variable_detail

/*!
 * Computation representing a single variable.
 *
 * When constructing the computation, the initial value of the variable must
 * be provided in a feature set implementing an `InitializationFeature`.
 * Otherwise, if the computation is default-constructed, the variable is
 * default-constructed.
 *
 * When the result of the computation is retrieved, a reference to the
 * variable is returned, with a `const` qualifier matching that of the
 * computation.
 *
 * If a function type is specified, a pointer to the function type is
 * stored instead. Storing array types is not supported; use `std::array`
 * instead.
 *
 * Computations created with this factory are implicitly `typed`.
 */
template <
    typename ImplementedFeature,
    typename Type,
    typename InitializationFeature
>
using variable = typename variable_detail::make_variable<
    ImplementedFeature, Type, InitializationFeature
>::type;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_VARIABLE_HPP
