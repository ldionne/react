#=============================================================================
# Setup project
#=============================================================================
project(react CXX)
cmake_minimum_required(VERSION 2.8)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(REACT_IS_NESTED FALSE)
else()
    set(REACT_IS_NESTED TRUE)
endif()


#=============================================================================
# Setup CMake options
#=============================================================================
include(CMakeDependentOption)
option(REACT_ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(REACT_ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(REACT_ENABLE_CXX0X    "Enable -std=c++0x if the compiler supports it." ON)
option(REACT_ENABLE_LIBCXX   "Enable -stdlib=libc++ if the compiler supports it." ON)
CMAKE_DEPENDENT_OPTION(
    REACT_ENABLE_TESTS "Enable the unit tests. To avoid target name conflicts and cluttering the list of targets, unit tests are disabled when react is nested inside another project."
    OFF "REACT_IS_NESTED" # off if REACT_IS_NESTED
    ON                    # on otherwise
)
CMAKE_DEPENDENT_OPTION(
    REACT_ENABLE_DOC "Enable the documentation target. To avoid target name conflicts and cluttering the list of targets, this is disabled when dyno is nested inside another project."
    OFF "REACT_IS_NESTED" # off if REACT_IS_NESTED
    ON                    # on otherwise
)


#=============================================================================
# Check available compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-pedantic                    REACT_HAS_PEDANTIC_FLAG)
check_cxx_compiler_flag(-std=c++0x                   REACT_HAS_STDCXX0X_FLAG)
check_cxx_compiler_flag(-stdlib=libc++               REACT_HAS_STDLIB_LIBCXX_FLAG)
check_cxx_compiler_flag(-W                           REACT_HAS_W_FLAG)
check_cxx_compiler_flag(-Wall                        REACT_HAS_WALL_FLAG)
check_cxx_compiler_flag(-Werror                      REACT_HAS_WERROR_FLAG)
check_cxx_compiler_flag(-Wextra                      REACT_HAS_WEXTRA_FLAG)
check_cxx_compiler_flag(-Wno-long-long               REACT_HAS_WNO_LONG_LONG_FLAG)
check_cxx_compiler_flag(-Wno-unused-local-typedefs   REACT_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG)
check_cxx_compiler_flag(-Wno-unused-parameter        REACT_HAS_WNO_UNUSED_PARAMETER_FLAG)
check_cxx_compiler_flag(-Wwrite-strings              REACT_HAS_WWRITE_STRINGS_FLAG)
check_cxx_compiler_flag(-ftemplate-backtrace-limit=0 REACT_HAS_FTEMPLATE_BACKTRACE_LIMIT_0)


#=============================================================================
# Setup warning flags
#=============================================================================
macro(append_if condition lst var)
    if (${condition})
        list(APPEND ${lst} ${var})
    endif()
endmacro()
append_if(REACT_HAS_WALL_FLAG                      REACT_CXX_WARNING_FLAGS -Wall)
append_if(REACT_HAS_WEXTRA_FLAG                    REACT_CXX_WARNING_FLAGS -Wextra)
append_if(REACT_HAS_W_FLAG                         REACT_CXX_WARNING_FLAGS -W)
append_if(REACT_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG REACT_CXX_WARNING_FLAGS -Wno-unused-local-typedefs)
append_if(REACT_HAS_WNO_UNUSED_PARAMETER_FLAG      REACT_CXX_WARNING_FLAGS -Wno-unused-parameter)
append_if(REACT_HAS_WWRITE_STRINGS_FLAG            REACT_CXX_WARNING_FLAGS -Wwrite-strings)
append_if(REACT_HAS_WNO_LONG_LONG_FLAG             REACT_CXX_WARNING_FLAGS -Wno-long-long)
append_if(REACT_HAS_FTEMPLATE_BACKTRACE_LIMIT_0    REACT_CXX_WARNING_FLAGS -ftemplate-backtrace-limit=0)

if (REACT_ENABLE_WERROR)
    append_if(REACT_HAS_WERROR_FLAG  REACT_CXX_WARNING_FLAGS -Werror)
    append_if(REACT_HAS_WX_FLAG      REACT_CXX_WARNING_FLAGS -WX)
endif()

if (REACT_ENABLE_PEDANTIC)
    append_if(REACT_HAS_PEDANTIC_FLAG REACT_CXX_WARNING_FLAGS -pedantic)
endif()


#=============================================================================
# Setup feature flags
#=============================================================================
if (REACT_ENABLE_CXX0X AND REACT_HAS_STDCXX0X_FLAG)
    list(APPEND REACT_CXX_FEATURE_FLAGS -std=c++0x)
endif()

if (REACT_ENABLE_LIBCXX AND REACT_HAS_STDLIB_LIBCXX_FLAG)
    list(APPEND REACT_CXX_FEATURE_FLAGS -stdlib=libc++)
    # the flag has to be passed to the linker too.
    set(CMAKE_EXE_LINKER_FLAGS -stdlib=libc++)
    set(CMAKE_SHARED_LINKER_FLAGS -stdlib=libc++)
    set(CMAKE_MODULE_LINKER_FLAGS -stdlib=libc++)
endif()

# This is the only place where add_definitions is called. Other properties
# are set on a per-target basis.
add_definitions(
    ${REACT_CXX_WARNING_FLAGS}
    ${REACT_CXX_FEATURE_FLAGS}
)


#=============================================================================
# Setup the documentation
#=============================================================================
if (REACT_ENABLE_DOC)
    add_subdirectory(doc)
endif()


#=============================================================================
# Setup tests
#=============================================================================
if (REACT_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
