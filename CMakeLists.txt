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
    VERSION 1.17.0
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

if(MSVC)
  set(MSVC_CRT /MD)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      message("Disabling Debug CRT")
      add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
      add_compile_options(/Od /Zi)
      foreach(MODE "_DEBUG" "_MINSIZEREL" "_RELEASE" "_RELWITHDEBINFO")
          string(REPLACE "${MSVC_CRT}d" "${MSVC_CRT}" TMP1 "${CMAKE_C_FLAGS${MODE}}")
          string(REPLACE "${MSVC_CRT}d" "${MSVC_CRT}" TMP2 "${CMAKE_CXX_FLAGS${MODE}}")
          set(CMAKE_C_FLAGS${MODE} "${TMP1}" CACHE STRING "" FORCE)
          set(CMAKE_CXX_FLAGS${MODE} "${TMP2}" CACHE STRING "" FORCE)
          #message(STATUS "C_${MODE}=${CMAKE_C_FLAGS${MODE}}")
          #message(STATUS "CXX_${MODE}=${CMAKE_CXX_FLAGS${MODE}}")
      endforeach(MODE)
  endif()
endif()

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64"))
    # -march=x86-64-v3: (close to Haswell) AVX, AVX2, BMI1, BMI2, F16C, FMA, LZCNT, MOVBE, XSAVE
    # -march=x86-64-v4: AVX512F, AVX512BW, AVX512CD, AVX512DQ, AVX512VL (generally not available on VM-s in 2023)
    # NOTE: on Alder Lake and later, AVX512 is no longer supported! So to support broken Intel CPU-s we cannot have AVX512
    add_compile_options(-march=x86-64-v3)
    message(STATUS "BUGFIX: Using -march=x86-64-v3 for backwards compatibility")
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
