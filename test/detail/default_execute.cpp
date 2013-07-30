/*!
 * @file
 * This file contains unit tests for the default implementation of
 * `react::execute`.
 */

#include <react/detail/default_execute.hpp>
#include <react/detail/auto_return.hpp>
#include <react/extensions/fusion.hpp>
#include <react/intrinsics.hpp>

#include <boost/fusion/include/clear.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_equal.hpp>
#include <utility>


using namespace react;

struct exec_as_void {
    template <typename Self, typename Env>
    static void execute(Self&&, Env&&) { }
};

struct exec_as_identity {
    template <typename Self, typename Env>
    static auto execute(Self&&, Env&& env)
    REACT_AUTO_RETURN(
        std::forward<Env>(env)
    )
};

template <typename Computation>
struct exec_with {
    template <typename Self, typename Env>
    static auto execute(Self&&, Env&& env)
    REACT_AUTO_RETURN(
        augment(std::forward<Env>(env), Computation{})
    )
};

struct no_execute { };

template <typename Sequence>
struct as_mpl_set
    : boost::mpl::fold<
        Sequence,
        boost::mpl::set<>,
        boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
    >
{ };

template <typename Computation>
struct executing {
    template <typename ...ComputationsAfterUpdate>
    static void should_yield() {
        using namespace boost;
        fusion::vector<Computation> env{};
        auto updated = execute(fusion::front(env), fusion::clear(env));

        using UpdatedEnv = decltype(updated);
        static_assert(mpl::set_equal<
            typename mpl::set<ComputationsAfterUpdate...>::type,
            typename as_mpl_set<UpdatedEnv>::type
        >::value, "");
    }
};

struct dummy_computation { };

int main() {
    executing<exec_as_void>::should_yield<exec_as_void>();
    executing<exec_as_identity>::should_yield<>();
    executing<exec_with<dummy_computation>>::should_yield<dummy_computation>();
    executing<no_execute>::should_yield<no_execute>();
}
