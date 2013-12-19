project(yggdrasil)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

include_directories(
    glew-1.9.0/include
    ${CMAKE_SOURCE_DIR}
)

file(
    COPY
    data
    DESTINATION
    ${EXECUTABLE_OUTPUT_PATH}
)

add_subdirectory(SFML)
add_subdirectory(Box2D_v2.2.1)
add_subdirectory(glew-1.9.0)
add_subdirectory(yggdrasil)
add_subdirectory(editor)
add_subdirectory(tinyxml)
