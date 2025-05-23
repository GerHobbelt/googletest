# Note: CMake support is community-based. The maintainers do not use CMake
# internally.

if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

cmake_minimum_required(VERSION 3.16)

project(googletest
    VERSION 1.16.0
    LANGUAGES CXX
)
include(CTest)
include(GNUInstallDirs)

set(CMAKE_DEBUG_POSTFIX "_d")

if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(GOOGLETEST_STANDALONE ON)
    message(STATUS "Standalone build detected")
else()
    set(GOOGLETEST_STANDALONE OFF)
    message(STATUS "Vendored build detected")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(INSTALL_GTEST "Install Googletest" ${GOOGLETEST_STANDALONE})
option(BUILD_GMOCK "Build googlemock" ON)

# Look for Python interpreter
if(CMAKE_VERSION VERSION_LESS "3.12.0")
  find_package(PythonInterp)
else()
  find_package(Python COMPONENTS Interpreter)
  set(PYTHONINTERP_FOUND ${Python_Interpreter_FOUND})
  set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
endif()

# Need /bigobj for tests
if(MSVC)
    add_compile_options(/bigobj)
endif()

# Helper functions
function(gtest_cxx_executable name lib)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE ${lib})
endfunction()

function(gtest_cxx_test name lib)
    gtest_cxx_executable(${name} ${lib} test/${name}.cc ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

function(gtest_py_test name)
    if (PYTHONINTERP_FOUND)
        if (CMAKE_CONFIGURATION_TYPES)
            # Multi-configuration build generators as for Visual Studio save
            # output in a subdirectory of CMAKE_CURRENT_BINARY_DIR (Debug,
            # Release etc.), so we have to provide it here.
            add_test(NAME ${name}
                COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/${name}.py
                    --build_dir=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> ${ARGN})
        else()
            # Single-configuration build generators like Makefile generators
            # don't have subdirs below CMAKE_CURRENT_BINARY_DIR.
            add_test(NAME ${name}
                COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/${name}.py
                    --build_dir=${CMAKE_CURRENT_BINARY_DIR} ${ARGN})
        endif()
    endif()
endfunction()

add_subdirectory(googletest)

if(GTEST_HAS_ABSL)
  if(NOT TARGET absl::base)
    find_package(absl REQUIRED)
  endif()
  if(NOT TARGET re2::re2)
    find_package(re2 REQUIRED)
  endif()
endif()

if(BUILD_GMOCK)
    add_subdirectory(googlemock)
endif()

if(INSTALL_GTEST)
    include(CMakePackageConfigHelpers)
    configure_package_config_file(cmake/Config.cmake.in GTestConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTest
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )
    write_basic_package_version_file(GTestConfigVersion.cmake COMPATIBILITY SameMajorVersion)
    install(EXPORT GTestTargets NAMESPACE GTest:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTest)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/GTestConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/GTestConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTest
    )
endif()
