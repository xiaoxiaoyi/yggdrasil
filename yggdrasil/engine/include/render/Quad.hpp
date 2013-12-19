// Yggdrasil Project (2013)
// Laurent "Otium" Noel | Vincent "Hekiat" Lesueur 
// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com

#ifndef _YGG_QUAD_HPP_
#define _YGG_QUAD_HPP_

#include <glm/glm.hpp>
#include <gloops/gloops.hpp>

namespace ygg {

class Quad {
public:
	struct Vertex {
		glm::vec2 position;
		
		Vertex(float x, float y):
			position(x, y) {
		}
	};

	void init();

    void bindVAO();

    void render();

private:
	gloops::BufferObject m_VBO;
	gloops::VertexArrayObject m_VAO;
};

}

#endif //_YGG_QUAD_HPP_

