/*!
 * @file
 * This file defines `react::factories::reference`.
 */

#ifndef REACT_FACTORIES_REFERENCE_HPP
#define REACT_FACTORIES_REFERENCE_HPP

#include <react/factories/value.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react { namespace factories {
    /*!
     * Metafunction creating a computation implemented as a reference to a
     * single value.
     *
     * This is equivalent to `value<boost::reference_wrapper<U>, Feature>`,
     * where `U` is `boost::remove_reference<T>::type`.
     */
    template <typename T, typename Feature>
    struct reference
        : value<
            boost::reference_wrapper<
                typename boost::remove_reference<T>::type
            >,
            Feature
        >
    { };
}} // end namespace react::factories

#endif // !REACT_FACTORIES_REFERENCE_HPP
