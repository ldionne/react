/*!
 * @file
 * This file defines `react::placeholder_for`.
 */

#ifndef REACT_PLACEHOLDER_FOR_HPP
#define REACT_PLACEHOLDER_FOR_HPP

#include <react/detail/computation_of.hpp>

#include <boost/mpl/bool.hpp>


namespace react {
    /*!
     * Placeholder with a special meaning when used inside `computation_set`.
     *
     * Its semantics is to be replaced by the `Computation` whose feature is
     * `Feature` inside the `computation_set`.
     */
    template <typename Feature>
    struct placeholder_for {
        template <typename Args, typename Kwargs>
        struct apply
            : detail::computation_of<Feature, Kwargs>
        { };
    };

    namespace detail { namespace custom_substitution_until_mpl11 {
        template <typename Feature>
        struct is_placeholder<placeholder_for<Feature>>
            : boost::mpl::true_
        { };
    }}
} // end namespace react

#endif // !REACT_PLACEHOLDER_FOR_HPP
