#include "render/shaders/debug_shaders.hpp"

namespace ygg {

const GLchar* DEBUG_VERTEX_SHADER_SRC =
YGG_SHADER_SOURCE(

layout(location = 0) in vec2 aPosition;

uniform mat4 uProjectionMatrix;
uniform mat3 uMVMatrix;

void main() {
    gl_Position = uProjectionMatrix * vec4(uMVMatrix * vec3(aPosition, 1), 1.f);
}

);

const GLchar* DEBUG_FRAGMENT_SHADER_SRC =
YGG_SHADER_SOURCE(

uniform vec3 uColor;

out vec3 fColor;

void main() {
    fColor = uColor;
}

);

}
