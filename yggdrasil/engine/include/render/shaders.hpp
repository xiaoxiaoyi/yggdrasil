// Yggdrasil Project (2013)
// Laurent "Otium" Noel | Vincent "Hekiat" Lesueur 
// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com

#ifndef _YGG_SHADERS_HPP_
#define _YGG_SHADERS_HPP_

#include <glm/glm.hpp>
#include <gloops/gloops.hpp>

#define YGG_SHADER_SOURCE(s) "#version 330\n" GLOOPS_STRINGIFY(s)

namespace ygg {

// Attribute locations
static const GLuint SHADER_ATTR_POSITION = 0;


// Uniform names
static const GLchar* SHADER_UNIFORM_PROJECTIONMATRIX = "uProjectionMatrix";
static const GLchar* SHADER_UNIFORM_MVMATRIX = "uMVMatrix";
static const GLchar* SHADER_UNIFORM_COLOR = "uColor";

}

#endif //_YGG_SHADERS_HPP_


