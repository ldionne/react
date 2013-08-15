/*!
 * @file
 * This file defines `react::computation::thread_local_`.
 */

#ifndef REACT_COMPUTATION_THREAD_LOCAL_HPP
#define REACT_COMPUTATION_THREAD_LOCAL_HPP

#include <react/computation/custom_stored.hpp>


namespace react { namespace computation {
    namespace thread_local_detail {
        template <typename T, typename MakeUnique>
        class get_thread_local {
            static T& thread_local_instance() {
                thread_local T x;
                return x;
            }

        public:
            T& operator()() { return thread_local_instance(); }
            T const& operator()() const { return thread_local_instance(); }
        };
    } // end namespace thread_local_detail

    /*!
     * Computation forwarding everything to another default-constructed
     * computation with `thread_local` storage duration.
     *
     * @tparam Computation
     *         The type of the computation to which everything shall be
     *         forwarded.
     *
     * @tparam MakeUnique
     *         A type used to control how the `Computation` instance is
     *         shared across instantiations of this template. By default,
     *         all instantiations with the same `Computation` type shall
     *         share the same instance.
     */
    template <typename Computation, typename MakeUnique = Computation>
    using thread_local_ = custom_stored<
        Computation,
        thread_local_detail::get_thread_local<Computation, MakeUnique>
    >;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_THREAD_LOCAL_HPP
