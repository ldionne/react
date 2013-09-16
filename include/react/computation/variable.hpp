/*!
 * @file
 * Defines `react::computation::variable`.
 */

#ifndef REACT_COMPUTATION_VARIABLE_HPP
#define REACT_COMPUTATION_VARIABLE_HPP

#include <react/computation/noop.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <array>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <type_traits>
#include <utility>


namespace react { namespace computation {
    namespace variable_detail {
        using namespace boost;

        template <typename Array>
        struct make_array {
            using type = std::array<
                typename std::remove_extent<Array>::type,
                std::extent<Array>::value
            >;
        };

        template <typename T>
        struct adjust
            : mpl::eval_if<std::is_function<T>, std::add_pointer<T>,
              mpl::eval_if<std::is_array<T>,    make_array<T>,
                           /* else */           mpl::identity<T>
            >>
        { };
    } // end namespace variable_detail

    /*!
     * Computation implemented as a single value.
     *
     * When the result of the computation is retrieved, a reference to the
     * value is returned, with a `const` qualifier matching that of the
     * computation.
     *
     * If a function type is specified, a pointer to the function type is
     * stored instead. If an array type is specified, a `std::array` is
     * stored instead.
     *
     * @tparam Type
     *         The type of the value to store.
     */
    template <typename Type>
    struct variable : noop {
    private:
        using AdjustedType = typename variable_detail::adjust<Type>::type;
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
