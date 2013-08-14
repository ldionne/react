/*!
 * @file
 * This file defines `react::computation::named`.
 */

#ifndef REACT_COMPUTATION_NAMED_HPP
#define REACT_COMPUTATION_NAMED_HPP

#include <react/computation/noop.hpp>


namespace react { namespace computation {
    /*!
     * Computation giving a specific name to a computation.
     *
     * @tparam Name
     *         The name representing the computation.
     *
     * @tparam Computation
     *         The computation to wrap in such a way that it has the name
     *         `Name`. It defaults to `noop`.
     */
    template <typename Name, typename Computation = noop>
    struct named : Computation {
        using Computation::Computation;
        using Computation::operator=;
        using name = Name;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_NAMED_HPP
