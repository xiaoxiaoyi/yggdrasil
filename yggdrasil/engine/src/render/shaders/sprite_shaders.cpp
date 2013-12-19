#include "render/shaders/sprite_shaders.hpp"

namespace ygg {

const GLchar* SPRITE_VERTEX_SHADER_SRC =
YGG_SHADER_SOURCE(

layout(location = 0) in vec2 aPosition;

uniform mat4 uProjectionMatrix;
uniform mat3 uMVMatrix;

uniform vec4 uTexCoordsTransform; // xy contains the origin and zw the scale (both in texture space)

out vec2 vTexCoords;

void main() {
    vec2 texCoords = vec2(0.5 + aPosition.x, 1 - 0.5 - aPosition.y);
    vTexCoords = uTexCoordsTransform.xy + uTexCoordsTransform.zw * texCoords;
    gl_Position = uProjectionMatrix * vec4(uMVMatrix * vec3(aPosition, 1), 1.f);
}

);

const GLchar* SPRITE_FRAGMENT_SHADER_SRC =
YGG_SHADER_SOURCE(

uniform sampler2D uSpriteTexture;

in vec2 vTexCoords;

out vec3 fColor;

void main() {
    vec4 color = texture(uSpriteTexture, vTexCoords);
    if(color.a == 0.f) {
        discard;
    }
    fColor = color.rgb;
}

);

}
