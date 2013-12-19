project(yggdrasil-engine)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-Wall -fPIC -ffast-math -fvisibility-inlines-hidden -fvisibility=hidden -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g -ggdb -O0 -ftree-ter")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -g -O2")
set(CMAKE_EXE_LINKER_FLAGS "")

find_package(OpenGL REQUIRED)
add_definitions(-DTW_STATIC)

# Include Headers Directories
include_directories(
    include/
    ${CMAKE_SOURCE_DIR}/glm-0.9.4.1
    ${CMAKE_SOURCE_DIR}/glew-1.9.0/include
    ${CMAKE_SOURCE_DIR}/gloops/include
    ${CMAKE_SOURCE_DIR}/Box2D_v2.2.1
    ${CMAKE_SOURCE_DIR}/tinyxml/
    ${CMAKE_SOURCE_DIR}/SFML/include/
    ${OPENGL_INCLUDE_DIR}
    AntTweakBar/include
)

# Get all source files
file(
	GLOB_RECURSE
	source_files
    src/*.cpp src/*.glsl include/*.hpp ${CMAKE_SOURCE_DIR}/gloops/include/*.hpp ${CMAKE_SOURCE_DIR}/boost/*.hpp
    AntTweakBar/*.cpp AntTweakBar/*.hpp AntTweakBar/*.h
)

add_library(
	yggdrasil-engine
    STATIC
	${source_files}
)

target_link_libraries(
    yggdrasil-engine
    Box2D
    glew
    ${OPENGL_LIBRARIES}
    tinyxml2
)
