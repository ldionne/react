/*!
 * @file
 * This file defines `react::computation::noop`.
 */

#ifndef REACT_COMPUTATION_NOOP_HPP
#define REACT_COMPUTATION_NOOP_HPP

#include <react/detail/dont_care.hpp>


namespace react { namespace computation {
    //! Computation that does nothing and yields no result (`void`).
    struct noop {
        inline static void execute(detail::dont_care, detail::dont_care) { }
        inline static void retrieve(detail::dont_care, detail::dont_care) { }
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_NOOP_HPP
