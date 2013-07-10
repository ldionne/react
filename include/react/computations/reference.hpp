/*!
 * @file
 * This file defines `react::computations::reference`.
 */

#ifndef REACT_COMPUTATIONS_REFERENCE_HPP
#define REACT_COMPUTATIONS_REFERENCE_HPP

#include <react/computations/value.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react { namespace computations {
    /*!
     * Computation implemented as a reference to a single value.
     *
     * This is equivalent to
     * `value<boost::reference_wrapper<U>, Feature, InitializationFeature>`,
     * where `U` is `boost::remove_reference<T>::type`.
     */
    template <typename T, typename Feature, typename InitializationFeature>
    using reference = value<
        boost::reference_wrapper<
            typename boost::remove_reference<T>::type
        >,
        Feature,
        InitializationFeature
    >;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_REFERENCE_HPP
