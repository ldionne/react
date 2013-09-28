/*!
 * @file
 * Defines `react::before`.
 */

#ifndef REACT_BEFORE_HPP
#define REACT_BEFORE_HPP

#include <boost/mpl/set.hpp>


namespace react {
    //! Sets the derived computation's successors to `Features...`.
    template <typename ...Features>
    struct before {
        using successors = typename boost::mpl::set<Features...>::type;
    };
} // end namespace react

#endif // !REACT_BEFORE_HPP
