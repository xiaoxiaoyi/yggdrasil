project(yggdrasil-editor)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-Wall -fPIC -ffast-math -fvisibility-inlines-hidden -fvisibility=hidden -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g -ggdb -O0 -ftree-ter")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -g -O2")
set(CMAKE_EXE_LINKER_FLAGS "")

set(QT5_INSTALL_DIR "$ENV{QT5_INSTALL_DIR}")
set(CMAKE_PREFIX_PATH "${QT5_INSTALL_DIR}")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenGL REQUIRED)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Usual include Directory
include_directories(
    include
    ${CMAKE_SOURCE_DIR}/glew-1.9.0/include
    ${CMAKE_SOURCE_DIR}/glm-0.9.4.1
    ${CMAKE_SOURCE_DIR}/tinyxml/
    ${CMAKE_SOURCE_DIR}/gloops/include
    ${CMAKE_SOURCE_DIR}/yggdrasil/engine/include
    ${OPENGL_INCLUDE_DIR}
)

# Copy UI Files
file(
    COPY
    ui
    DESTINATION
    ${EXECUTABLE_OUTPUT_PATH}
)

file(GLOB_RECURSE FILES_HPP  "include/*.hpp"  )
file(GLOB_RECURSE FILES_CPP  "src/*.cpp")
file(GLOB_RECURSE FILES_UI   "ui/*.ui" )
file(GLOB_RECURSE FILES_QRC  "data/*.qrc")
file(GLOB_RECURSE FILES_QML  "ui/*.qml")

qt5_wrap_ui(FILES_HPP ${FILES_UI})
qt5_add_resources(FILES_QRC ${FILES_QRC})

# Tell CMake to create the helloworld executable
add_executable(editor ${FILES_CPP} ${FILES_HPP} ${FILES_QRC} ${FILES_QML})

qt5_use_modules(editor Widgets OpenGL Gui Declarative)

target_link_libraries(editor ${Qt5Widgets_LIBRARIES} ${OPENGL_LIBRARIES} glew tinyxml2 yggdrasil-engine)
