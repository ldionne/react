/*!
 * @file
 * Defines `react::computation::lazy`.
 */

#ifndef REACT_COMPUTATION_LAZY_HPP
#define REACT_COMPUTATION_LAZY_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <utility>


namespace react { namespace computation {
    namespace lazy_detail {
        template <typename Computation, typename DefaultEnv>
        class defer_retrieval {
            Computation computation_;
            DefaultEnv default_env_;

        public:
            template <typename C, typename E>
            explicit defer_retrieval(C&& c, E&& env)
                : computation_(std::forward<C>(c)),
                  default_env_(std::forward<E>(env))
            { }

            template <typename Env>
            auto operator()(Env&& env) const REACT_AUTO_RETURN(
                retrieve(computation_, std::forward<Env>(env))
            )

            auto operator()() const REACT_AUTO_RETURN(
                retrieve(computation_, default_env_)
            )
        };
    } // end namespace lazy_detail

    /*!
     * Computation delaying the actual retrieval of its result.
     *
     * When the result of a `lazy` computation is retrieved, a proxy is
     * returned. When that proxy is called, the actual result of the
     * computation is retrieved. If an environment is provided to the
     * proxy when it is called, that environment is used instead to
     * perform the retrieval.
     *
     * @tparam Computation
     *         The computation whose result is to be retrieved lazily.
     */
    template <typename Computation>
    struct lazy : Computation {
    private:
        // Avoid `using Computation::Computation`, which makes us unable to
        // use `Computation` to refer to the template parameter with Clang.
        using Computation_ = Computation;

    public:
        using Computation_::Computation_;
        using Computation::operator=;

        template <typename Env>
        static auto retrieve(lazy& self, Env&& env) REACT_AUTO_RETURN(
            lazy_detail::defer_retrieval<Computation&, Env>{
                self, std::forward<Env>(env)
            }
        )

        template <typename Env>
        static auto retrieve(lazy const& self, Env&& env) REACT_AUTO_RETURN(
            lazy_detail::defer_retrieval<Computation const&, Env>{
                self, std::forward<Env>(env)
            }
        )

        template <typename Env>
        static auto retrieve(lazy&& self, Env&& env) REACT_AUTO_RETURN(
            lazy_detail::defer_retrieval<Computation, Env>{
                std::move(self), std::forward<Env>(env)
            }
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_LAZY_HPP
