cmake_minimum_required(VERSION 3.7)
set(PROJECT_NAME RTSPServer)
project(${PROJECT_NAME})

option(DEBUG "release or debug" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cpplint)

if (DEBUG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    add_definitions(-DNDEBUG)
endif ()

find_package(PkgConfig)
if (NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config needed, install it please!")
endif ()

pkg_check_modules(libavformat REQUIRED libavformat)
pkg_check_modules(libavutil REQUIRED libavutil)
pkg_check_modules(libavcodec REQUIRED libavcodec)
pkg_check_modules(spdlog REQUIRED spdlog)
pkg_check_modules(fmt REQUIRED fmt)    # fmt 5.3.0 issue on macOS 10.14.4, use as submodule

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66.0 REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost needed, install it please!!!")
endif ()

include_directories(${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}/third_party
        ${libavformat_INCLUDE_DIRS}
        ${libavutil_INCLUDE_DIRS}
        ${libavcodec_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${spdlog_INCLUDE_DIRS}
        ${fmt_INCLUDE_DIRS}
        )

link_directories(${libavformat_LIBRARY_DIRS}
        ${libavutil_LIBRARY_DIRS}
        ${libavcodec_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
        ${spdlog_LIBRARY_DIRS}
        ${fmt_LIBRARY_DIRS}
        )

add_subdirectory(common)
add_subdirectory(third_party)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(testProgs)