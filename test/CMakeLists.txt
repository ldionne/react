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
        APPEND PROPERTY INCLUDE_DIRECTORIES ${react_SOURCE_DIR}/include
    )
    set_property(TARGET ${name}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${react_SOURCE_DIR}/ext/mpl_extensions/include
    )
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(unit ${name})
endfunction()


react_add_test(archetypes                 archetypes.cpp)
react_add_test(detail_cloaked_feature_set detail/cloaked_feature_set.cpp)
react_add_test(detail_feature_set_by_ref  detail/feature_set_by_ref.cpp)
react_add_test(detail_feature_set_union   detail/feature_set_union.cpp)
react_add_test(feature_set_concept        feature_set/concept.cpp)
react_add_test(master_header              master_header.cpp)
