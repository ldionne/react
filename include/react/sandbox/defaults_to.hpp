/*!
 * @file
 * Defines `react::defaults_to`.
 */

#ifndef REACT_DEFAULTS_TO_HPP
#define REACT_DEFAULTS_TO_HPP

#include <boost/mpl/identity.hpp>


namespace react {
    /*!
     * Sets the derived requirement's default unbound computation
     * to `DefaultUnboundComputation`.
     */
    template <typename DefaultUnboundComputation>
    struct defaults_to {
        struct apply
            : boost::mpl::identity<DefaultUnboundComputation>
        { };
    };
} // end namespace react

#endif // !REACT_DEFAULTS_TO_HPP
