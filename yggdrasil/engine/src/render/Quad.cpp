// Yggdrasil Project (2013)
// Laurent "Otium" Noel | Vincent "Hekiat" Lesueur 
// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com

#include "render/Quad.hpp"
#include "render/shaders.hpp"

namespace ygg {

void Quad::init() {
	Vertex vertices[] = {
		Vertex(-0.5f, -0.5f),
		Vertex(0.5f, -0.5f),
		Vertex(0.5f,  0.5f),
		Vertex(-0.5f,  0.5f)
	};
	
	gloops::BufferBind vbo(GL_ARRAY_BUFFER, m_VBO);
		vbo.bufferData(4, vertices, GL_STATIC_DRAW);
	
	gloops::VertexArrayBind vao(m_VAO);
		vao.enableVertexAttribArray(SHADER_ATTR_POSITION);
		vao.vertexAttribPointer(
			SHADER_ATTR_POSITION,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			GLOOPS_OFFSETOF(Vertex, position)
		);
}

void Quad::bindVAO() {
    glBindVertexArray(m_VAO.glId());
}

void Quad::render() {
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

}

