/*!
 * @file
 * Defines `react::can_be_bound`.
 */

#ifndef REACT_CAN_BE_BOUND_HPP
#define REACT_CAN_BE_BOUND_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl11/operator/and.hpp>


namespace react {
    /*!
     * Returns whether an unbound computation can be bound with the
     * provided `FeatureMap`.
     */
    template <typename UnboundComputation, typename FeatureMap>
    struct can_be_bound
        : boost::mpl::true_
    { };

    template <template <typename ...> class F, typename ...T,
              typename FeatureMap>
    struct can_be_bound<F<T...>, FeatureMap>
        : boost::mpl11::and_<
            can_be_bound<T, FeatureMap>...,
            boost::mpl::true_,
            boost::mpl::true_
        >::type
    { };
} // end namespace react

#endif // !REACT_CAN_BE_BOUND_HPP
