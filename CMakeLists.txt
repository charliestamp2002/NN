cmake_minimum_required(VERSION 3.20)
project(NeuralNet
    VERSION 0.1.0
    DESCRIPTION "Neural network from scratch in C++"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Build with AddressSanitizer + UBSan" OFF)
option(BUILD_TESTS       "Build the unit tests"                ON)

# Warnings we want on code (deliberately not forced onto deps like Catch2).
set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic)

if(ENABLE_SANITIZERS)
    message(STATUS "Sanitizers enabled: address, undefined")
    add_compile_options(-fsanitize=address,undefined -g -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(nn_lib
    src/matrix.cpp
    )

target_include_directories(nn_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(neural_net
    src/main.cpp
    )

target_link_libraries(neural_net PRIVATE nn_lib)
target_compile_options(nn_lib PRIVATE ${PROJECT_WARNINGS})

if (BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(unit_tests  
        tests/test_matrix.cpp
        )
    target_link_libraries(unit_tests PRIVATE nn_lib Catch2::Catch2WithMain)
    target_compile_options(unit_tests PRIVATE ${PROJECT_WARNINGS})
    enable_testing()
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(unit_tests)
endif()





