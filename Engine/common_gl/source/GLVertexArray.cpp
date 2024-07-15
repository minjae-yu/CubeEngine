//Author: JEYOON YU
//Project: CubeEngine
//File: GLVertexArray.cpp
#include "GLVertexArray.hpp"

#include "glCheck.hpp"

GLVertexArray::~GLVertexArray()
{
	glCheck(glDeleteVertexArrays(1, &vaoHandle));
}

void GLVertexArray::Initialize()
{
	glCheck(glCreateVertexArrays(1, &vaoHandle));
}

void GLVertexArray::AddVertexBuffer(GLVertexBuffer&& buffer, size_t size, std::initializer_list<GLAttributeLayout> layout)
{
	GLuint vboHandle = buffer.GetHandle();

	//bind a buffer to a vertex buffer bind point
	glCheck(glVertexArrayVertexBuffer(vaoHandle, 0, vboHandle, 0, static_cast<GLsizei>(size)));

	for (const GLAttributeLayout& attribute : layout)
	{
		glCheck(glEnableVertexArrayAttrib(vaoHandle, attribute.vertex_layout_location));
		glCheck(glVertexArrayAttribFormat(vaoHandle, attribute.vertex_layout_location, attribute.component_dimension, attribute.component_type, attribute.normalized, attribute.relative_offset));
		glCheck(glVertexArrayAttribBinding(vaoHandle, attribute.vertex_layout_location, 0));
	}
	buffers.push_back(std::move(buffer));
}

void GLVertexArray::SetIndexBuffer(GLIndexBuffer&& buffer)
{
	numIndices = buffer.GetCount();
	indexBuffer = std::move(buffer);
	glCheck(glVertexArrayElementBuffer(vaoHandle, indexBuffer.GetHandle()));
}

void GLVertexArray::Use(bool bind)
{
	if (bind == true)
	{
		glCheck(glBindVertexArray(vaoHandle));
	}
	else if (bind == false)
	{
		glCheck(glBindVertexArray(0));
	}
}
