/*!
 * @file
 * This file defines `react::computation::temporary`.
 */

#ifndef REACT_COMPUTATION_TEMPORARY_HPP
#define REACT_COMPUTATION_TEMPORARY_HPP

#include <react/computation/ephemeral.hpp>


namespace react { namespace computation {
    template <typename Computation>
    using temporary = ephemeral<Computation, 1>;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_TEMPORARY_HPP
