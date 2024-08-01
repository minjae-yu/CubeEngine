//Author: JEYOON YU
//Second Author: DOYEONG LEE
//Project: CubeEngine
//File: VKRenderManager.hpp
#pragma once
#include <SDL2/SDL_vulkan.h>
#include <array>

#include "RenderManager.hpp"
#include "VKDescriptor.hpp"
#include "VKTexture.hpp"
#include "VKVertexBuffer.hpp"
#include "VKIndexBuffer.hpp"
#include "VKUniformBuffer.hpp"
#include "VKImGuiManager.hpp"

const auto IMAGE_AVAILABLE_INDEX{ 0 };
const auto RENDERING_DONE_INDEX{ 1 };

class Window;
class VKInit;
class VKSwapChain;
class VKShader;
class VKPipeLine;
template<typename Material>
class VKUniformBuffer;

class VKRenderManager : public RenderManager
{
public:
	VKRenderManager() { gMode = GraphicsMode::VK; };
	~VKRenderManager();
	void Initialize(SDL_Window* window_);

	void BeginRender(glm::vec4 bgColor) override;
	void EndRender() override;
	//void EndRender(Window* window_);
private:
	void InitCommandPool();
	void InitCommandBuffer();
	void InitRenderPass();
	void InitFrameBuffer(VkExtent2D* swapchainImageExtent_, std::vector<VkImageView>* swapchainImageViews_);
	void CleanSwapChain();
	void RecreateSwapChain();

	SDL_Window* window;
	VKInit* vkInit;
	VKSwapChain* vkSwapChain;

	VkCommandPool vkCommandPool{ VK_NULL_HANDLE };
	std::array<VkCommandBuffer, 2> vkCommandBuffers{ VK_NULL_HANDLE };
	VkRenderPass vkRenderPass{ VK_NULL_HANDLE };
	std::vector<VkFramebuffer> vkFrameBuffers;

	//Depth Buffering
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties properties;
			vkGetPhysicalDeviceFormatProperties(*vkInit->GetPhysicalDevice(), format, &properties);

			if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}
		throw std::runtime_error("Failed To Find Supported Format!");
	};
	VkFormat FindDepthFormat()
	{
		return FindSupportedFormat
		(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}
	bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	};
	uint32_t FindMemoryTypeIndex(const VkMemoryRequirements requirements_, VkMemoryPropertyFlags properties_)
	{
		//Get Physical Device Memory Properties
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(*vkInit->GetPhysicalDevice(), &physicalDeviceMemoryProperties);

		//Find memory type index which satisfies both requirement and property
		for (uint32_t i = 0; i != physicalDeviceMemoryProperties.memoryTypeCount; ++i)
		{
			//Check if memory is allocatable at ith memory type
			if (!(requirements_.memoryTypeBits & (1 << i)))
				continue;

			//Check if satisfies memory property
			if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties_) != properties_)
				continue;

			return i;
		}
		return UINT32_MAX;
	}

#ifdef _DEBUG
	VKImGuiManager* imguiManager;
#endif

	VKShader* vkShader2D;
	VKPipeLine* vkPipeline2D;
	VKDescriptor* vkDescriptor;

	uint32_t swapchainIndex;
	VkImage swapchainImage;
	std::array<VkSemaphore, 2> vkSemaphores;
	uint32_t frameIndex{ 0 };
	bool isRecreated{ false };

	VkCommandBuffer* currentCommandBuffer{ VK_NULL_HANDLE };
	VkFence* currentFence{ VK_NULL_HANDLE };
	VkDescriptorSet* currentVertexMaterialDescriptorSet{ VK_NULL_HANDLE };
	VkDescriptorSet* currentTextureDescriptorSet{ VK_NULL_HANDLE };
public:
	//--------------------Common--------------------//
	void DeleteWithIndex(int id) override;

	//--------------------2D Render--------------------//
	void LoadTexture(const std::filesystem::path& path_, std::string name_) override;
	void LoadQuad(glm::vec4 color_, float isTex_, float isTexel_) override;

	VKTexture* GetTexture(std::string name);

	//--------------------3D Render--------------------//
	void LoadMesh(MeshType type, glm::vec4 color, int stacks, int slices) override;
private:
	//--------------------Common--------------------//
	VKIndexBuffer* indexBuffer{ nullptr };

	//--------------------2D Render--------------------//
	std::vector<VKTexture*> textures;
	std::vector<VkDescriptorImageInfo> imageInfos;

	VKVertexBuffer<TwoDimension::Vertex>* vertex2DBuffer{ nullptr };

	VKUniformBuffer<TwoDimension::VertexUniform>* vertex2DUniform{ nullptr };
	VKUniformBuffer<TwoDimension::FragmentUniform>* fragment2DUniform{ nullptr };

	//--------------------3D Render--------------------//
};
