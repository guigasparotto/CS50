cmake_minimum_required(VERSION 3.25)
project(CS50 C)

set(CMAKE_C_STANDARD 11)

# Only include utils.c in utils library
add_library(utils utils/utils.c)

add_subdirectory(readability)
add_subdirectory(substitution)
add_subdirectory(search-algos)
add_subdirectory(sort-algos)

# Add a custom target that depends on your other targets
add_custom_target(CS50 ALL
        DEPENDS readability substitution)