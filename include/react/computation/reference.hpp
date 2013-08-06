/*!
 * @file
 * This file defines `react::computation::reference`.
 */

#ifndef REACT_COMPUTATION_REFERENCE_HPP
#define REACT_COMPUTATION_REFERENCE_HPP

#include <react/computation/variable.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react { namespace computation {
/*!
 * Computation implemented as a reference to a single variable.
 *
 * This is equivalent to `variable<boost::reference_wrapper<U>>`, where `U`
 * is `boost::remove_reference<Type>::type`.
 */
template <typename Type>
using reference = variable<
    boost::reference_wrapper<typename boost::remove_reference<Type>::type>
>;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_REFERENCE_HPP
