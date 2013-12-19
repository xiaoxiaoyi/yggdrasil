project(yggdrasil-game)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-Wall -fPIC -ffast-math -fvisibility-inlines-hidden -fvisibility=hidden -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g -ggdb -O0 -ftree-ter")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -g -O2")
set(CMAKE_EXE_LINKER_FLAGS "")

find_package(OpenGL REQUIRED)

# Include Headers Directories
include_directories(
    include/
    ${CMAKE_SOURCE_DIR}/glm-0.9.4.1
    ${CMAKE_SOURCE_DIR}/glew-1.9.0/include
    ${CMAKE_SOURCE_DIR}/gloops/include
    ${CMAKE_SOURCE_DIR}/Box2D_v2.2.1
    ${CMAKE_SOURCE_DIR}/SFML/include
    ${CMAKE_SOURCE_DIR}/yggdrasil/engine/include
    ${CMAKE_SOURCE_DIR}/yggdrasil/engine/AntTweakBar/include
    ${CMAKE_SOURCE_DIR}/tinyxml
)

# Get all source files
file(
	GLOB_RECURSE
	source_files
    src/*.cpp src/*.c include/*.hpp
)

add_executable(
	yggdrasil-game
	${source_files}
)

target_link_libraries(
    yggdrasil-game
    glew
    sfml-graphics
    sfml-window 
    sfml-system
    yggdrasil-engine
    ${OPENGL_LIBRARIES} 
)
