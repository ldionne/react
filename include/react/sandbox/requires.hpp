/*!
 * @file
 * Defines `react::requires`.
 */

#ifndef REACT_REQUIRES_HPP
#define REACT_REQUIRES_HPP

#include <boost/mpl/set.hpp>


namespace react {
    //! Sets the derived computation's requirements to `Requirements...`.
    template <typename ...Requirements>
    struct requires {
        using requirements = typename boost::mpl::set<Requirements...>::type;
    };
} // end namespace react

#endif // !REACT_REQUIRES_HPP
