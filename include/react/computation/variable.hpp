/*!
 * @file
 * This file defines `react::computation::variable`.
 */

#ifndef REACT_COMPUTATION_VARIABLE_HPP
#define REACT_COMPUTATION_VARIABLE_HPP

#include <react/computation/noop.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/array.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/extent.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <utility>


namespace react { namespace computation {
    namespace variable_detail {
        template <typename Array>
        struct make_array {
            using type = boost::array<
                typename boost::remove_extent<Array>::type,
                boost::extent<Array>::value
            >;
        };
    } // end namespace variable_detail

    /*!
     * Computation representing a single value.
     *
     * When the result of the computation is retrieved, a reference to the
     * value is returned, with a `const` qualifier matching that of the
     * computation.
     *
     * If a function type is specified, a pointer to the function type is
     * stored instead. If an array type is specified, a `boost::array` is
     * stored instead.
     *
     * @tparam Type
     *         The type of the value to store.
     */
    template <typename Type>
    struct variable : noop {
    private:
        using AdjustedType = typename
        boost::mpl::eval_if<boost::is_function<Type>,
            boost::add_pointer<Type>,
        boost::mpl::eval_if<boost::is_array<Type>,
            variable_detail::make_array<Type>,
        /* else */
            boost::mpl::identity<Type>
        >>::type;

        AdjustedType var_;

    public:
        template <typename ...Args>
        explicit variable(Args&& ...args)
            : var_(std::forward<Args>(args)...)
        { }

        static auto retrieve(variable& self, detail::dont_care)
        REACT_AUTO_RETURN(self.var_)

        static auto retrieve(variable const& self, detail::dont_care)
        REACT_AUTO_RETURN(self.var_)

        static auto retrieve(variable&& self, detail::dont_care)
        REACT_AUTO_RETURN(std::move(self.var_))
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_VARIABLE_HPP
