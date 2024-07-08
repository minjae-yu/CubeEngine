//Author: JEYOON YU
//Project: CubeEngine
//File: GLUniformBuffer.hpp
#include "GLUniformBuffer.hpp"

template <typename Material>
GLUniformBuffer<Material>::~GLUniformBuffer()
{
	glCheck(glDeleteBuffers(1, &uniformHandle));
}
