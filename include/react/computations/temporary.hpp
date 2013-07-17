/*!
 * @file
 * This file defines `react::computations::temporary`.
 */

#ifndef REACT_COMPUTATIONS_TEMPORARY_HPP
#define REACT_COMPUTATIONS_TEMPORARY_HPP

#include <react/computations/ephemeral.hpp>


namespace react { namespace computations {
    template <typename Computation>
    using temporary = ephemeral<Computation, 1>;
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_TEMPORARY_HPP
