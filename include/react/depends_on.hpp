/*!
 * @file
 * This file defines `react::depends_on`.
 */

#ifndef REACT_DEPENDS_ON_HPP
#define REACT_DEPENDS_ON_HPP

#include <boost/mpl/set.hpp>


namespace react {
    template <typename ...Dependencies>
    struct depends_on {
        using dependencies = boost::mpl::set<Dependencies...>;
    };
}

#endif // !REACT_DEPENDS_ON_HPP
