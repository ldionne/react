/*!
 * @file
 * This file defines `react::computations::reference`.
 */

#ifndef REACT_COMPUTATIONS_REFERENCE_HPP
#define REACT_COMPUTATIONS_REFERENCE_HPP

#include <react/computations/variable.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react { namespace computations {
/*!
 * Computation implemented as a reference to a single variable.
 *
 * This is equivalent to
 * @code
 *  variable<
 *      boost::reference_wrapper<U>,
 *      ImplementedFeature,
 *      InitializationFeature
 *  >
 * @endcode
 * where `U` is `boost::remove_reference<Type>::type`.
 */
template <
    typename Type,
    typename ImplementedFeature,
    typename InitializationFeature
>
using reference = variable<
    boost::reference_wrapper<
        typename boost::remove_reference<Type>::type
    >,
    ImplementedFeature,
    InitializationFeature
>;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_REFERENCE_HPP
