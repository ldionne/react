/*!
 * @file
 * This file contains unit tests for `react::factories::value`.
 */

#include <react/factories/value.hpp>
#include <react/concepts.hpp>
#include <react/feature_sets/from_argument_pack.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/mpl/if.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_function.hpp>
#include <string>
#include <type_traits>
#include <utility>


using namespace react;

struct null_concept { BOOST_CONCEPT_USAGE(null_concept) { } };

template <typename T>
struct test_concept {
    using ValueComputation = typename factories::value<
        T, feature_archetype<>
    >::type;

    BOOST_CONCEPT_USAGE(test_concept) {
        auto keyword = boost::parameter::keyword<feature_archetype<>>::get();

        BOOST_CONCEPT_ASSERT((IncrementalComputation<
            ValueComputation,
            decltype(feature_sets::make_from_argument_pack(
                keyword = std::declval<T>()
            )),
            semantic_tags<>,
            dependency_results<>
        >));

        BOOST_CONCEPT_ASSERT((typename boost::mpl::if_<
            std::is_default_constructible<T>,
            IncrementalComputation<
                ValueComputation,
                default_construct,
                semantic_tags<>,
                dependency_results<>
            >,
            null_concept
        >::type));

        // If we want the
        // `ValueComputation::result_type v = value_comp.result(env)`
        // expression to be valid, we require `T` to be convertible
        // to `ValueComputation::result_type`.
        using AdjustedT = typename boost::mpl::if_<
            boost::is_function<T>, typename boost::add_pointer<T>::type, T
        >::type;
        static_assert(boost::is_convertible<
            AdjustedT, typename ValueComputation::result_type
        >::value, "");
    }
};


// Check with primitive types, user-defined types and function types.
BOOST_CONCEPT_ASSERT((test_concept<int>));
BOOST_CONCEPT_ASSERT((test_concept<std::string>));
BOOST_CONCEPT_ASSERT((test_concept<int*>));
BOOST_CONCEPT_ASSERT((test_concept<void()>));

BOOST_CONCEPT_ASSERT((test_concept<int const>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const>));
BOOST_CONCEPT_ASSERT((test_concept<int* const>));


// Check with [const] references.
BOOST_CONCEPT_ASSERT((test_concept<int&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string&>));
BOOST_CONCEPT_ASSERT((test_concept<int* &>));
BOOST_CONCEPT_ASSERT((test_concept<void(&)()>));
BOOST_CONCEPT_ASSERT((test_concept<int(&)[2]>));

BOOST_CONCEPT_ASSERT((test_concept<int const&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const&>));
BOOST_CONCEPT_ASSERT((test_concept<int* const&>));


// Small runtime test just to make sure the value is set alright.
BOOST_PARAMETER_KEYWORD(tag, my_value)
using MyValueFeature = factories::value<std::string, tag::my_value>::type;

struct dont_care { };

int main() {
    MyValueFeature f{feature_sets::make_from_argument_pack(my_value = "abcd")};

    std::string abcd_value = f.result(dont_care{});
    BOOST_ASSERT(abcd_value == "abcd");

    std::string& abcd_ref = f.result(dont_care{});
    BOOST_ASSERT(abcd_ref == "abcd");

    std::string const& abcd_cref = f.result(dont_care{});
    BOOST_ASSERT(abcd_cref == "abcd");
}
