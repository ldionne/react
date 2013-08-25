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
        APPEND PROPERTY INCLUDE_DIRECTORIES ${react_SOURCE_DIR}/include
    )
    set_property(TARGET ${name}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${react_SOURCE_DIR}/ext/mpl_extensions/include
    )
    set_property(TARGET ${name}
        APPEND PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
    )
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(unit ${name})
endfunction()


react_add_test(archetypes                   archetypes.cpp)
react_add_test(computation_alias            computation/alias.cpp)
react_add_test(computation_custom_stored    computation/custom_stored.cpp)
react_add_test(computation_executable_if    computation/executable_if.cpp)
react_add_test(computation_executed_after   computation/executed_after.cpp)
react_add_test(computation_executed_before  computation/executed_before.cpp)
react_add_test(computation_implementing     computation/implementing.cpp)
react_add_test(computation_lazy             computation/lazy.cpp)
react_add_test(computation_noop             computation/noop.cpp)
react_add_test(computation_requiring        computation/requiring.cpp)
react_add_test(computation_retrievable_if   computation/retrievable_if.cpp)
react_add_test(computation_static           computation/static.cpp)
react_add_test(computation_strict           computation/strict.cpp)
react_add_test(computation_thread_local     computation/thread_local.cpp)
react_add_test(computation_union            computation/union.cpp)
react_add_test(computation_variable         computation/variable.cpp)
react_add_test(detail_computation_map       detail/computation_map.cpp)
react_add_test(detail_flatten_into_set      detail/flatten_into_set.cpp)
react_add_test(detail_topological_indexing  detail/topological_indexing.cpp)
react_add_test(extension_fusion             extension/fusion.cpp)
react_add_test(master_header                master_header.cpp)
