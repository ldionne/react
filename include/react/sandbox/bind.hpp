/*!
 * @file
 * Defines `react::bind`.
 */

#ifndef REACT_BIND_HPP
#define REACT_BIND_HPP

#include <boost/mpl11/functional/apply.hpp>


namespace react {
    /*!
     * Creates a fully functional computation from an unbound computation
     * and a feature map.
     *
     * @return A @ref Computation
     */
    template <typename UnboundComputation, typename FeatureMap>
    struct bind
        : boost::mpl11::apply<UnboundComputation, FeatureMap>
    { };
} // end namespace react

#endif // !REACT_BIND_HPP
