/*!
 * @file
 * This file defines `react::feature_set`.
 */

#ifndef REACT_FEATURE_SET_HPP
#define REACT_FEATURE_SET_HPP

#include <react/archetypes.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/call_computation.hpp>
#include <react/detail/cloaked_feature_set.hpp>
#include <react/detail/feature_dependency_graph.hpp>
#include <react/detail/feature_set_by_ref.hpp>
#include <react/detail/pointers_to.hpp>
#include <react/feature_set_union.hpp>
#include <react/traits.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/topological_sort.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>
#include <utility>


namespace react {
namespace feature_set_detail {
namespace mpl = boost::mpl;

struct last_storage_node {
    template <typename ...Args>
    last_storage_node(Args&& ...) { }

    template <typename Anything, bool always_false = false>
    void at(Anything&&) const {
        static_assert(always_false,
        "could not find the requested feature in the graph");
    }
};

template <typename Computation, typename Next>
struct storage_node : Next {
    storage_node() = default;

    template <typename Args>
    explicit storage_node(Args const& args, typename boost::enable_if<
        std::is_constructible<Computation, Args const&>
    >::type* = 0)
        : Next{args}, computation_{args}
    { }

    template <typename Args>
    explicit storage_node(Args const& args, typename boost::disable_if<
        std::is_constructible<Computation, Args const&>
    >::type* = 0)
        : Next{args}, computation_{}
    { }

    using Next::at;
    Computation& at(typename feature_of<Computation>::type const&) {
        return computation_;
    }

private:
    Computation computation_;
};

template <typename Vertex>
struct computation_from_vertex {
    using type = typename Vertex::computation;
};

template <typename ...Computations>
class feature_set {
private:
    //! Map associating features whose implementation was explicitly
    //! specified in the feature set to that implementation.
    using PredefinedComputations = mpl::map<
        mpl::pair<typename feature_of<Computations>::type, Computations>...
    >;

    template <typename Feature>
    struct computation_of
        : mpl::eval_if<
            mpl::has_key<PredefinedComputations, Feature>,
            mpl::at<PredefinedComputations, Feature>,
            mpl::apply<Feature>
        >
    { };

    using DependencyGraph = detail::feature_dependency_graph<
        computation_of<mpl::_1>, mpl::vector<Computations...>
    >;

public:
    using computations = typename mpl::fold<
        typename mpl::vertices_of<DependencyGraph>::type,
        mpl::set<>,
        mpl::insert<mpl::_1, computation_from_vertex<mpl::_2>>
    >::type;


private:
    using Storage = typename mpl::inherit_linearly<
        typename mpl::vertices_of<DependencyGraph>::type,
        storage_node<computation_from_vertex<mpl::_2>, mpl::_1>,
        last_storage_node
    >::type;
    Storage features_;

public:
    feature_set() = default;

    template <typename Args>
    explicit feature_set(Args const& args)
        : features_{args}
    { }


private:
    using FeaturesInVisitationOrder = mpl::transform_view<
        typename mpl::topological_sort<DependencyGraph>::type,
        feature_of<computation_from_vertex<mpl::_1>>
    >;

    template <typename SemanticTag, typename Union>
    struct computation_executer {
        feature_set& self;
        Union union_;
        template <typename Feature>
        void operator()(Feature* feature) {
            detail::call_computation<SemanticTag>(
                self.features_.at(*feature), cloak_for(*feature, union_)
            );
        }
    };

    template <typename SemanticTag, typename Union>
    computation_executer<SemanticTag, Union> execute_computation(Union&& u) {
        return {*this, u};
    }

public:
    template <typename SemanticTag, typename FeatureSet>
    void operator()(SemanticTag const& tag, FeatureSet&& ext) {
        mpl::for_each<
            typename detail::pointers_to<FeaturesInVisitationOrder>::type
        >(
            execute_computation<SemanticTag>(
                make_feature_set_union(*this, std::forward<FeatureSet>(ext))
            )
        );
    }


private:
    template <typename Feature>
    struct computation_at
        : boost::remove_reference<decltype(
            std::declval<Storage&>().at(std::declval<Feature const&>())
        )>
    { };

    template <typename FeatureSet, typename Feature>
    static detail::cloaked_feature_set<
        detail::feature_set_by_ref<FeatureSet>,
        typename computation_at<Feature>::type
    > cloak_for(Feature const&, FeatureSet&& fset) {
        return {std::forward<FeatureSet>(fset)};
    }

public:
    template <typename SemanticTag>
    void operator()(SemanticTag const& tag) {
        return operator()(tag, feature_set<>{});
    }

    template <typename Feature>
    auto operator[](Feature const& feature) REACT_AUTO_RETURN(
        features_.at(feature).result(cloak_for(feature, *this))
    )
};

template <typename ...Computations>
struct pseudo_root_computation : incremental_computation_archetype<> {
    using dependencies = mpl::set<typename feature_of<Computations>::type...>;
};
} // end namespace feature_set_detail

template <typename ...Computations>
using feature_set = detail::cloaked_feature_set<
    feature_set_detail::feature_set<Computations...>,
    feature_set_detail::pseudo_root_computation<Computations...>
>;
} // end namespace react

#endif // !REACT_FEATURE_SET_HPP
