cmake_minimum_required(VERSION 3.10)

project(blink1-lib VERSION 1.0.0)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_PRINTF)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
option(BLINK1_LIB_BUILD_STATIC no)
option(BLINK1_LIB_BUILD_SHARED no)

if ((HAS_PARENT OR BLINK1_LIB_BUILD_STATIC) AND NOT BLINK1_LIB_BUILD_SHARED)
    set(LIBRARY_TYPE STATIC)
elseif ((NOT HAS_PARENT OR BLINK1_LIB_BUILD_SHARED) AND NOT BLINK1_LIB_BUILD_STATIC)
    set(LIBRARY_TYPE SHARED)
else()
    message(FATAL_ERROR "Must set exactly 1 of BLINK1_LIB_BUILD_SHARED or BLINK1_LIB_BUILD_STATIC, or neither of them.")
endif()

message("-- blink1-lib library type: ${LIBRARY_TYPE}")

if (HAS_PARENT)
    set(INCLUDE_DIR_SYSTEM SYSTEM)
else(HAS_PARENT)
    set(INCLUDE_DIR_SYSTEM "")
endif(HAS_PARENT)

##############
# BLINK1-LIB #
##############
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAG_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(INCLUDES ${INCLUDE_DIR})

set(SOURCES
    ${SOURCE_DIR}/Blink1Device.cpp
    ${SOURCE_DIR}/PatternLine.cpp
    ${SOURCE_DIR}/PatternLineN.cpp
    ${SOURCE_DIR}/PlayState.cpp
    ${SOURCE_DIR}/RGB.cpp
    ${SOURCE_DIR}/RGBN.cpp
)

set(CXX_STANDARD_REQUIRED yes)

add_library(blink1 ${LIBRARY_TYPE} ${SOURCES})
add_library(blink1-testing ${LIBRARY_TYPE} ${SOURCES} ${SOURCE_DIR}/Blink1TestingLibrary.cpp)

set_property(TARGET blink1 PROPERTY CXX_STANDARD 20)
set_property(TARGET blink1-testing PROPERTY CXX_STANDARD 20)

###############
# BLINK1-TOOL #
###############
set(BLINK1_DIR ${LIB_DIR}/blink1-tool)
set(LIB_BLINK1_LOC ${BLINK1_DIR}/libblink1.so)
set(INCLUDES ${INCLUDES} ${BLINK1_DIR} ${BLINK1_DIR}/hidapi/hidapi)

add_definitions(-DUSE_HIDAPI)

add_custom_command(OUTPUT ${LIB_BLINK1_LOC}
                   COMMAND $(MAKE) lib
                   WORKING_DIRECTORY ${BLINK1_DIR})
add_custom_target(libblink1_target DEPENDS ${LIB_BLINK1_LOC})
add_library(libblink1 STATIC IMPORTED GLOBAL)
set_target_properties(libblink1 PROPERTIES IMPORTED_LOCATION ${LIB_BLINK1_LOC})
add_dependencies(libblink1 libblink1_target)

add_dependencies(blink1 libblink1)

target_include_directories(blink1 ${INCLUDE_DIR_SYSTEM} PUBLIC ${INCLUDES})
target_include_directories(blink1-testing ${INCLUDE_DIR_SYSTEM} PUBLIC ${INCLUDES})

if(NOT HAS_PARENT)

    ###########
    # DOXYGEN #
    ###########
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${LIB_DIR}/doxygen-awesome-css/doxygen-awesome.css")
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        doxygen_add_docs(
            doxygen
            ${INCLUDE_DIR}
            COMMENT "Generate documentation"
        )
    endif(DOXYGEN_FOUND)

    ###########
    # Testing #
    ###########
    add_subdirectory(${LIB_DIR}/googletest)
    include(GoogleTest)
    set(TEST_EXECUTABLE_NAME blink1_lib_test)
    set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

    set(TEST_SOURCES
        ${TEST_SOURCE_DIR}/Blink1Device_BadInit_test.cpp
        ${TEST_SOURCE_DIR}/Blink1Device_Blocking_test.cpp
        ${TEST_SOURCE_DIR}/Blink1Device_GoodInit_test.cpp
        ${TEST_SOURCE_DIR}/Blink1Device_GoodInitBadFunction_test.cpp
        ${TEST_SOURCE_DIR}/PatternLineN_test.cpp
        ${TEST_SOURCE_DIR}/PatternLine_test.cpp
        ${TEST_SOURCE_DIR}/PlayState_test.cpp
        ${TEST_SOURCE_DIR}/RGBN_test.cpp
        ${TEST_SOURCE_DIR}/RGB_test.cpp
    )

    enable_testing()
    add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES})
    target_link_libraries(${TEST_EXECUTABLE_NAME} blink1-testing gtest gtest_main gmock gmock_main)
    gtest_discover_tests(${TEST_EXECUTABLE_NAME})
    set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

    set(WARNINGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Set specific warnings if they ever exist
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(WARNINGS "${WARNINGS} -Wmisleading-indentation")
    endif()
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "${WARNINGS}")

    ############
    # COVERAGE #
    ############
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
        include(CodeCoverage)

        set(COVERAGE_EXCLUDES "/usr/*" "${LIB_DIR}/*" "${TEST_SOURCE_DIR}/*")
        APPEND_COVERAGE_COMPILER_FLAGS()

        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(GCOV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts/llvm-cov.sh")
            SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage
                                           EXECUTABLE ${TEST_EXECUTABLE_NAME}
                                           DEPENDENCIES ${TEST_EXECUTABLE_NAME})
        else()
            SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage
                                                 EXECUTABLE ${TEST_EXECUTABLE_NAME}
                                                 DEPENDENCIES ${TEST_EXECUTABLE_NAME})
        endif()

        add_definitions(-DDEBUG_PRINTF)

        set_target_properties(
            ${EXECUTABLE_NAME}
            PROPERTIES
                CXX_CLANG_TIDY "clang-tidy"
        )

    endif (CMAKE_BUILD_TYPE MATCHES Debug)

    ################
    # OPTIMIZATION #
    ################
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT error)
    if (ipo_supported)
        message(STATUS "IPO/LTO enabled")
        set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO/LTO not supported")
    endif()

endif(NOT HAS_PARENT)
