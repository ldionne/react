#=============================================================================
# Setup unit tests
#=============================================================================
find_package(Boost 1.53 REQUIRED)

add_custom_target(unit COMMENT "build all the unit tests")
add_custom_target(check
    DEPENDS unit
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "build and then run all the unit tests"
)

function(react_add_test name sources)
    add_executable(${name} EXCLUDE_FROM_ALL ${sources})
    set_property(TARGET ${name}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
    )
    set_property(TARGET ${name}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${react_SOURCE_DIR}/ext/mpl_extensions/include
    )
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(unit ${name})
endfunction()


react_add_test(archetypes                      archetypes.cpp)
react_add_test(computations_alias              computations/alias.cpp)
react_add_test(computations_from_fusion_map    computations/from_fusion_map.cpp)
react_add_test(computations_lazy               computations/lazy.cpp)
react_add_test(computations_reference          computations/reference.cpp)
react_add_test(computations_typed              computations/typed.cpp)
react_add_test(computations_variable           computations/variable.cpp)
react_add_test(feature_sets_by_ref             feature_sets/by_ref.cpp)
react_add_test(feature_sets_cloaked            feature_sets/cloaked.cpp)
react_add_test(feature_sets_default            feature_sets/default.cpp)
react_add_test(feature_sets_empty              feature_sets/empty.cpp)
react_add_test(feature_sets_from_argument_pack feature_sets/from_argument_pack.cpp)
react_add_test(feature_sets_union              feature_sets/union.cpp)
react_add_test(master_header                   master_header.cpp)
