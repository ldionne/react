/*!
 * @file
 * This file defines `react::computation::named`.
 */

#ifndef REACT_COMPUTATION_NAMED_HPP
#define REACT_COMPUTATION_NAMED_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace computation {
    namespace named_detail {
        struct as_base;

        template <typename T>
        using raw = typename boost::remove_cv<
            typename boost::remove_reference<T>::type
        >::type;
    }

    template <typename Name, typename Computation = named_detail::as_base>
    struct named : Computation {
        using Computation::Computation;

        template <typename C, typename = typename boost::enable_if<
            boost::is_same<Computation, named_detail::raw<C>>
        >::type>
        named(C&& c)
            : Computation(std::forward<C>(c))
        { }

        using Computation::operator=;
        using name = Name;
    };

    template <typename Name>
    struct named<Name, named_detail::as_base> {
        using name = Name;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_NAMED_HPP
