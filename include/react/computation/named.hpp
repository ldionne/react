/*!
 * @file
 * This file defines `react::computation::named`.
 */

#ifndef REACT_COMPUTATION_NAMED_HPP
#define REACT_COMPUTATION_NAMED_HPP

namespace react { namespace computation {
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
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_NAMED_HPP
