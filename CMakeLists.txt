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
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

############################### COMBILER OPTIONS #######################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # TODO: Add Release flags subset
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        -O0 -g -ggdb
        -std=c++17
        -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-function
    )
else()
    message(FATAL_ERROR "[BMGUI] Unsupported compiler!")
endif()

############################### DEFINITIONS ############################################################################

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DPATH_TO_DATA="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

############################### EXTERNAL LIBRARIES #####################################################################

target_link_libraries(${PROJECT_NAME} fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/fmt)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmt/include)

option(CORE_LIBRARY_SHARED "" ON)
target_link_libraries(${PROJECT_NAME} core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/core/src)

if (LINK_OPENGL)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    message(STATUS "[BMGUI] Linking with OpenGL: ${OPENGL_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})

    find_package(GLEW REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
    message(STATUS "[BMGUI] Linking with GLEW: ${GLEW_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
endif()

if (LINX_SFML)
    find_package(SFML 2 REQUIRED COMPONENTS window system graphics)
    target_link_libraries(${PROJECT_NAME} -lsfml-window -lsfml-graphics -lsfml-system ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
    message(STATUS "[SANDBOX] Linking with SFML")

    target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
endif()
