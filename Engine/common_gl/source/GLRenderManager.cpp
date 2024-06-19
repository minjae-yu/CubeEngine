//Author: JEYOON YU
//Project: CubeEngine
//File: GLRenderManager.cpp
#include "GLRenderManager.hpp"

GLRenderManager::~GLRenderManager()
{
}

void GLRenderManager::Initialize()
{
	vertexArray.Initialize();
	//shader.LoadShader({ {GLShader::VERTEX, "../Engine/shader/texVertex.vert"}, { GLShader::FRAGMENT, "../Engine/shader/texFragment.frag" } });
}

void GLRenderManager::BeginRender()
{
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//shader.Use();
	//for (auto& tex : textures)
	//{
	//	tex->UseForSlot(1);
	//}

	//uVertex->SendUniform(shader.GetProgramHandle(), 0, "vUniformMatrix", vertexVector);
	//uFragment->SendUniform(shader.GetProgramHandle(), 0, "fUniformMatrix", fragVector);

	//vertexArray.Use(true);
	//GLDrawIndexed(vertexArray);
}

void GLRenderManager::EndRender()
{
	//vertexArray.Use(false);
	//shader.Use(false);
}

void GLRenderManager::LoadTexture(const std::filesystem::path& path_, std::string name_)
{
	GLTexture* texture = new GLTexture;
	texture->LoadTexture(path_, name_);

	textures.push_back(texture);

	int texId = static_cast<int>(textures.size() - 1);
	textures.at(texId)->SetTextureID(texId);
}

void GLRenderManager::LoadQuad(glm::vec4 color_, float isTex_, float isTexel_)
{
	texVertices.push_back(GLVertex(glm::vec4(-1.f, 1.f, 1.f, 1.f), quadCount));
	texVertices.push_back(GLVertex(glm::vec4(1.f, 1.f, 1.f, 1.f), quadCount));
	texVertices.push_back(GLVertex(glm::vec4(1.f, -1.f, 1.f, 1.f), quadCount));
	texVertices.push_back(GLVertex(glm::vec4(-1.f, -1.f, 1.f, 1.f), quadCount));
	if (texVertex != nullptr)
		delete texVertex;
	texVertex = new GLVertexBuffer(static_cast<GLsizei>(sizeof(GLVertex) * texVertices.size()));
	texVertex->SetData(texVertices.data());

	uint64_t indexNumber{ texVertices.size() / 4 - 1 };
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber));
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber + 1));
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber + 2));
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber + 2));
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber + 3));
	texIndices.push_back(static_cast<uint16_t>(4 * indexNumber));
	if (texIndex != nullptr)
		delete texIndex;
	texIndex = new GLIndexBuffer(&texIndices);

	quadCount++;

	//Attributes
	GLAttributeLayout position_layout;
	position_layout.component_type = GLAttributeLayout::Float;
	position_layout.component_dimension = GLAttributeLayout::_4;
	position_layout.normalized = false;
	position_layout.vertex_layout_location = 0;
	position_layout.stride = sizeof(glm::vec4);
	position_layout.offset = 0;
	position_layout.relative_offset = offsetof(GLVertex, position);

	GLAttributeLayout index_layout;
	position_layout.component_type = GLAttributeLayout::Int;
	position_layout.component_dimension = GLAttributeLayout::_1;
	position_layout.normalized = false;
	position_layout.vertex_layout_location = 1;
	position_layout.stride = sizeof(int);
	position_layout.offset = 0;
	position_layout.relative_offset = offsetof(GLVertex, index);

	vertexArray.AddVertexBuffer(std::move(*texVertex), { position_layout, index_layout });
	vertexArray.SetIndexBuffer(std::move(*texIndex));

	//if (uVertex != nullptr)
	//	delete uVertex;
	//uVertex = new VKUniformBuffer<VertexUniform>(&Engine::Instance().GetVKInit(), quadCount);

	//if (uFragment != nullptr)
	//	delete uFragment;
	//uFragment = new VKUniformBuffer<FragmentUniform>(&Engine::Instance().GetVKInit(), quadCount);

	GLVertexUniform mat;
	mat.model = glm::mat4(1.f);
	mat.view = glm::mat4(1.f);
	mat.projection = glm::mat4(1.f);
	vertexVector.push_back(mat);
	vertexVector.back().color = color_;
	vertexVector.back().isTex = isTex_;
	vertexVector.back().isTexel = isTexel_;

	GLFragmentUniform tIndex;
	tIndex.texIndex = 0;
	fragVector.push_back(tIndex);
}

void GLRenderManager::DeleteWithIndex()
{
	quadCount--;

	if (quadCount == 0)
	{
		texVertices.erase(end(texVertices) - 4, end(texVertices));
		delete texVertex;
		texVertex = nullptr;

		texIndices.erase(end(texIndices) - 6, end(texIndices));
		delete texIndex;
		texIndex = nullptr;

		vertexVector.erase(end(vertexVector) - 1);
		//delete uVertex;
		//uVertex = nullptr;

		fragVector.erase(end(fragVector) - 1);
		//delete uFragment;
		//uFragment = nullptr;

		return;
	}

	texVertices.erase(end(texVertices) - 4, end(texVertices));

	texIndices.erase(end(texIndices) - 6, end(texIndices));

	vertexVector.erase(end(vertexVector) - 1);
	//for (auto u : *uVertex->GetUniformBuffers())
	//{
	//	vkCmdUpdateBuffer(commandBuffer, u, 0, quadCount * sizeof(VertexUniform), vertexVector.data());
	//}

	fragVector.erase(end(fragVector) - 1);
	//for (auto u : *uFragment->GetUniformBuffers())
	//{
	//	vkCmdUpdateBuffer(commandBuffer, u, 0, quadCount * sizeof(FragmentUniform), fragVector.data());
	//}
}
