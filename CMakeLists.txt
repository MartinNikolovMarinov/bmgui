cmake_minimum_required(VERSION 3.0)

project(bmgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json

################################ OPTIONS ###############################################################################

option(LINK_OPENGL "Use OpenGL" ON)
option(LINX_SFML "Use SFML" ON)

################################ SOURCES ###############################################################################

# Common files
set(SRC_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

############################### COMBILER OPTIONS #######################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # TODO: Add Release flags subset
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        -O0 -g -ggdb
        -std=c++17
        -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-function
    )
else()
    message(FATAL_ERROR "[SANDBOX] Unsupported compiler!")
endif()

############################### DEFINITIONS ###########################################################################

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DPATH_TO_DATA="${CMAKE_CURRENT_SOURCE_DIR}/data"
)
