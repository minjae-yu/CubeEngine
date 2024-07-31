//Author: JEYOON YU
//Project: CubeEngine
//File: GLRenderManager.hpp
#pragma once
#include "glCheck.hpp"
#include "RenderManager.hpp"
#include "GLShader.hpp"
#include "GLVertexArray.hpp"
#include "GLTexture.hpp"
#include "GLUniformBuffer.hpp"
#include "GLImGuiManager.hpp"

class GLRenderManager : public RenderManager
{
public:
	GLRenderManager() { gMode = GraphicsMode::GL; };
	~GLRenderManager();
	void Initialize(
#ifdef _DEBUG
		SDL_Window* window_, SDL_GLContext context_
#endif
	);

	void BeginRender(glm::vec4 bgColor) override;
	void EndRender() override;
private:
	void GLDrawIndexed(const GLVertexArray& vertex_array) noexcept
	{
		glCheck(glDrawElements(GL_TRIANGLES, vertex_array.GetIndicesCount(), GL_UNSIGNED_SHORT, 0));
	}

	void GLDrawVertices(const GLVertexArray& vertex_array) noexcept
	{
		glCheck(glDrawArrays(GL_TRIANGLES, 0, vertex_array.GetVerticesCount()));
	}

	GLVertexArray vertexArray;
	GLShader gl2DShader;
#ifdef _DEBUG
	GLImGuiManager* imguiManager;
#endif
public:
	//--------------------Common--------------------//
	void DeleteWithIndex() override;

	//--------------------2D Render--------------------//
	void LoadTexture(const std::filesystem::path& path_, std::string name_) override;
	void LoadQuad(glm::vec4 color_, float isTex_, float isTexel_) override;

	GLTexture* GetTexture(std::string name);

	//--------------------3D Render--------------------//
	void LoadMesh(MeshType type) override;
private:
	//--------------------Common--------------------//
	GLVertexBuffer* vertexBuffer{ nullptr };
	GLIndexBuffer* indexBuffer{ nullptr };

	//--------------------2D Render--------------------//
	std::vector<GLTexture*> textures;
	std::vector<int> samplers;

	GLUniformBuffer<TwoDimension::VertexUniform>* vertexUniform2D{ nullptr };
	GLUniformBuffer<TwoDimension::FragmentUniform>* fragmentUniform2D{ nullptr };

	//--------------------3D Render--------------------//
};
