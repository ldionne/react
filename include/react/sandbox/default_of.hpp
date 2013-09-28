/*!
 * @file
 * Defines `react::default_of`.
 */

#ifndef REACT_DEFAULT_OF_HPP
#define REACT_DEFAULT_OF_HPP

#include <react/detail/either.hpp>

#include <boost/mpl11/empty_base.hpp>
#include <boost/mpl11/functional/apply.hpp>


namespace react {
    /*!
     * Returns the default implementation of a requirement (optionally
     * parameterized with `Args...`) if such a default is provided.
     *
     * If provided, the default implementation is merely a suggestion;
     * a different computation may implement that requirement's feature
     * depending on the context.
     *
     * By default, `Requirement` is considered as a Boost.MPL Metafunction
     * Class to which `Args...` is applied. If `Requirement` is not a valid
     * Metafunction Class, no nested `type` is provided, allowing SFINAE
     * to be used with this metafunction.
     *
     * @return An @ref UnboundComputation
     */
    template <typename Requirement, typename ...Args>
    struct default_of
        : detail::either<
            boost::mpl11::apply<Requirement, Args...>,
            boost::mpl11::empty_base
        >
    { };

    //! Equivalent to `default_of<Requirement, Args...>`.
    template <typename Requirement, typename ...Args>
    struct default_of<Requirement(Args...)>
        : default_of<Requirement, Args...>
    { };
} // end namespace react

#endif // !REACT_DEFAULT_OF_HPP
