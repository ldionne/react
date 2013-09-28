/*!
 * @file
 * Defines `react::after`.
 */

#ifndef REACT_AFTER_HPP
#define REACT_AFTER_HPP

#include <boost/mpl/set.hpp>


namespace react {
    //! Sets the derived computation's predecessors to `Features...`.
    template <typename ...Features>
    struct after {
        using predecessors = typename boost::mpl::set<Features...>::type;
    };
} // end namespace react

#endif // !REACT_AFTER_HPP
