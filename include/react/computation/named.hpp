/*!
 * @file
 * This file defines `react::computation::named`.
 */

#ifndef REACT_COMPUTATION_NAMED_HPP
#define REACT_COMPUTATION_NAMED_HPP

#include <react/computation/noop.hpp>
#include <react/intrinsic/name_of.hpp>

#include <boost/mpl/always.hpp>


namespace react {
namespace computation {
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
} // end namespace computation

namespace extension {
    template <typename Name, typename Computation>
    struct name_of_impl<computation::named<Name, Computation>>
        : boost::mpl::always<Name>
    { };
} // end namespace extension
} // end namespace react

#endif // !REACT_COMPUTATION_NAMED_HPP
