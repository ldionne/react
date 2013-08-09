/*!
 * @file
 * This file defines the `react::bind` intrinsic.
 */

#ifndef REACT_INTRINSIC_BIND_HPP
#define REACT_INTRINSIC_BIND_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/name_of.hpp>
#include <react/tag_of.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react {
namespace extension {
    template <typename Tag, typename Enable = void>
    struct bind_impl {
        template <typename Name, typename Env, typename Computation,
                  bool always_false = false>
        static void call(Env&&, Computation&&) {
            static_assert(always_false,
            "There is no default implementation for "
            "`react::bind<Name>(Environment, Computation)`.");
        }
    };
} // end namespace extension

namespace bind_detail {
    struct default_name;

    template <typename Name>
    struct do_bind {
        template <typename Env, typename Computation>
        static auto call(Env&& env, Computation&& c) REACT_AUTO_RETURN(
            extension::bind_impl<
                typename tag_of<Env>::type
            >::template call<Name>(
                std::forward<Env>(env), std::forward<Computation>(c)
            )
        )
    };

    template <>
    struct do_bind<default_name> {
        template <typename Env, typename Computation>
        static auto call(Env&& env, Computation&& c) REACT_AUTO_RETURN(
            do_bind<
                typename name_of<
                    typename boost::remove_reference<Computation>::type
                >::type
            >::call(std::forward<Env>(env), std::forward<Computation>(c))
        )
    };
} // end namespace bind_detail

template <typename Name = bind_detail::default_name,
          typename Env, typename Computation>
auto bind(Env&& env, Computation&& c) REACT_AUTO_RETURN(
    bind_detail::do_bind<Name>::call(
        std::forward<Env>(env), std::forward<Computation>(c)
    )
)
} // end namespace react

#endif // !REACT_INTRINSIC_BIND_HPP
