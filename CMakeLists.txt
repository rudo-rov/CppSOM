cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

project(SOM CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

link_libraries(stdc++fs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE /home/rudo/Downloads/DP_Rovnak_2021/src/antlr-4.9.2-complete.jar)

set(ANTLR4_WITH_STATIC_CRT ON)

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)
antlr_target(SOMLexer ${PROJECT_SOURCE_DIR}/antlr_grammar/SOMLexer.g4 LEXER PACKAGE antlr_som)
antlr_target(SOMParser ${PROJECT_SOURCE_DIR}/antlr_grammar/SOMParser.g4 PARSER PACKAGE antlr_som
        DEPENDS_ANTLR SOMLexer VISITOR COMPILE_FLAGS -lib ${ANTLR_SOMLexer_OUTPUT_DIR})

# include generated files in project environment
include_directories(${ANTLR_SOMLexer_OUTPUT_DIR} ${ANTLR_SOMParser_OUTPUT_DIR})
add_library(antlr_som ${ANTLR_SOMLexer_CXX_OUTPUTS} ${ANTLR_SOMParser_CXX_OUTPUTS})

target_compile_options(antlr_som PUBLIC)
set_property(TARGET antlr_som PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
target_link_libraries(antlr_som antlr4_static)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
