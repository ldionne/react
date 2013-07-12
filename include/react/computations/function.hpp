/*!
 * @file
 * This file defines `react::computations::function`.
 */

#ifndef REACT_COMPUTATIONS_FUNCTION_HPP
#define REACT_COMPUTATIONS_FUNCTION_HPP

#include <react/depends_on.hpp>
#include <react/detail/auto_return.hpp>
#include <react/implements.hpp>


namespace react { namespace computations {
/*!
 * Computation whose `result()` is implemented by a function call.
 *
 * The function is provided through a `FunctionFeature` that must be present
 * in the feature set when retrieving the result of this computation. After
 * retrieving the function from the feature set, it is called with the result
 * of the features represented by `Args...` in the feature set.
 *
 * The computation can't be executed.
 */
template <
    typename ImplementedFeature,
    typename FunctionFeature,
    typename ...Args
>
struct function
    : implements<ImplementedFeature>,
      depends_on<FunctionFeature, Args...>
{
    function() = default;

    template <typename FeatureSet>
    auto result(FeatureSet& fset) const REACT_AUTO_RETURN(
        fset[FunctionFeature{}](fset[Args{}]...)
    )
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_FUNCTION_HPP
