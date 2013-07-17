/*!
 * @file
 * This file defines `react::computations::named`.
 */

#ifndef REACT_COMPUTATIONS_NAMED_HPP
#define REACT_COMPUTATIONS_NAMED_HPP

namespace react { namespace computations {
    namespace named_detail {
        struct as_base;
    }

    template <typename Name, typename Computation = named_detail::as_base>
    struct named : Computation {
        using Computation::Computation;
        using Computation::operator=;
        using name = Name;
    };

    template <typename Name>
    struct named<Name, named_detail::as_base> {
        using name = Name;
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_NAMED_HPP
