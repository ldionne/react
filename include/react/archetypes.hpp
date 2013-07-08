/*!
 * @file
 * This file defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>


namespace react {

    template <typename Base = boost::null_archetype<>>
    struct composite_feature_archetype;

    template <typename Base = boost::null_archetype<>>
    struct feature_archetype;

    template <typename Base = boost::null_archetype<>>
    struct incremental_computation_archetype;

    template <typename Base = boost::null_archetype<>>
    struct feature_set_archetype;

//////////////////////////////////////////////////////////////////////////////

    template <typename Base>
    struct composite_feature_archetype : Base {
        using subfeatures = boost::mpl::set<>;
    };

//////////////////////////////////////////////////////////////////////////////

    //! Archetype for the `Feature` concept.
    template <typename Base>
    struct feature_archetype : Base {
        struct apply {
            using type = incremental_computation_archetype<>;
        };
    };

//////////////////////////////////////////////////////////////////////////////

    //! Archetype for the `IncrementalComputation` concept.
    template <typename Base>
    struct incremental_computation_archetype : Base {
        incremental_computation_archetype();

        template <typename Args>
        explicit incremental_computation_archetype(Args&);

        template <typename Tag, typename FeatureSet>
        boost::null_archetype<>& operator()(Tag const&, FeatureSet&);

        template <typename FeatureSet>
        boost::null_archetype<>& result(FeatureSet&);

        using dependencies = boost::mpl::set<>;
        using feature = feature_archetype<>;
    };

//////////////////////////////////////////////////////////////////////////////

    //! Archetype for the `FeatureSet` concept.
    template <typename Base>
    struct feature_set_archetype : Base {
        feature_set_archetype();

        template <typename Args>
        explicit feature_set_archetype(Args&);

        template <typename Tag>
        boost::null_archetype<>& operator()(Tag const&);

        template <typename Tag, typename FeatureSet>
        boost::null_archetype<>& operator()(Tag const&, FeatureSet& ext);

        template <typename Feature>
        boost::null_archetype<>& operator[](Feature const&);
    };

} // end namespace react

#endif // !REACT_ARCHETYPES_HPP
