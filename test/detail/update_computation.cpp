/*!
 * @file
 * This file contains unit tests for `react::detail::update_computation`.
 */

#include <react/detail/update_computation.hpp>
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

struct update_as_void {
    template <typename Env>
    void update(Env&&) const { }
};

struct update_as_identity {
    template <typename Env>
    auto update(Env&& env) const
    REACT_AUTO_RETURN(
        std::forward<Env>(env)
    )
};

template <typename Computation>
struct update_with {
    template <typename Env>
    auto update(Env&& env) const
    REACT_AUTO_RETURN(
        augment(std::forward<Env>(env), Computation{})
    )
};

struct no_update { };

template <typename Sequence>
struct as_mpl_set
    : boost::mpl::fold<
        Sequence,
        boost::mpl::set<>,
        boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
    >
{ };

template <typename Computation>
struct updating {
    template <typename ...ComputationsAfterUpdate>
    static void should_yield() {
        using namespace boost;
        fusion::vector<Computation> env{};
        auto updated = react::detail::update_computation(
                                    fusion::clear(env), fusion::front(env));

        using UpdatedEnv = decltype(updated);
        static_assert(mpl::set_equal<
            typename mpl::set<ComputationsAfterUpdate...>::type,
            typename as_mpl_set<UpdatedEnv>::type
        >::value, "");
    }
};

struct dummy_computation { };

int main() {
    updating<update_as_void>::should_yield<update_as_void>();
    updating<update_as_identity>::should_yield<>();
    updating<update_with<dummy_computation>>::should_yield<dummy_computation>();
    updating<no_update>::should_yield<no_update>();
}
