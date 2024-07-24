//Author: DOYEONG LEE
//Second Author: JEYOON YU
//Project: CubeEngine
//File: Sprtie.cpp
#include "BasicComponents/Sprite.hpp"
#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

Sprite::~Sprite()
{
	for (Animation* anim : animations)
	{
		delete anim;
	}
	animations.clear();
	if (this != nullptr)
	{
		Engine::GetSpriteManager().DeleteSprite(this);
	}
}

void Sprite::Init()
{
}

void Sprite::Update(float dt)
{
	UpdateProjection();
	UpdateView();
	UpdateModel(GetOwner()->GetPosition(), GetOwner()->GetSize(), GetOwner()->GetRotate());

	UpdateAnimation(dt);
}

void Sprite::End()
{
	Engine::GetSpriteManager().DeleteSprite(this);
}

void Sprite::UpdateModel(glm::vec3 pos_, glm::vec3 size_, float angle)
{
	glm::mat4 modelMatrix(1.0f);
	glm::vec3 pos;
	switch (Engine::GetRenderManager()->GetGraphicsMode())
	{
	case GraphicsMode::GL:
		pos = glm::vec3(pos_.x * 2, pos_.y * 2, pos_.z);
		modelMatrix = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size_.x, size_.y, size_.z));
		break;
	case GraphicsMode::VK:
		pos = glm::vec3(pos_.x * 2, -pos_.y * 2, pos_.z);
		modelMatrix = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size_.x, size_.y, size_.z));
		break;
	}
	Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).model = modelMatrix;
}

void Sprite::UpdateView()
{
	Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).view = Engine::GetCameraManager().GetViewMatrix();
}

void Sprite::UpdateProjection()
{
	Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).projection = Engine::GetCameraManager().GetProjectionMatrix();
}

void Sprite::AddQuad(glm::vec4 color_)
{
	Engine::Instance().GetRenderManager()->LoadQuad(color_, 0.f, 0.f);
	materialId = static_cast<int>(Engine::Instance().GetRenderManager()->GetVertexVector()->size() - 1);
	AddSpriteToManager();
}

//void Sprite::AddQuadLine(glm::vec4 color_)
//{
//	Engine::Instance().GetVKRenderManager().LoadLineQuad(color_);
//	materialId = static_cast<int>(Engine::Instance().GetVKRenderManager().GetVertexVector()->size() - 1);
//	AddSpriteToManager();
//}

void Sprite::AddMeshWithTexture(std::string name_, glm::vec4 color_)
{
	RenderManager* renderManager = Engine::Instance().GetRenderManager();
	renderManager->LoadQuad(color_, 1.f, 0.f);
	materialId = static_cast<int>(renderManager->GetVertexVector()->size() - 1);
	ChangeTexture(name_);
	switch (renderManager->GetGraphicsMode())
	{
	case GraphicsMode::GL:
		textureSize = dynamic_cast<GLRenderManager*>(renderManager)->GetTexture(name_)->GetSize();
		break;
	case GraphicsMode::VK:
		textureSize = dynamic_cast<VKRenderManager*>(renderManager)->GetTexture(name_)->GetSize();
		break;
	}
	AddSpriteToManager();
}

void Sprite::AddMeshWithTexel(std::string name_, glm::vec4 color_)
{
	RenderManager* renderManager = Engine::Instance().GetRenderManager();
	renderManager->LoadQuad(color_, 1.f, 1.f);
	materialId = static_cast<int>(renderManager->GetVertexVector()->size() - 1);
	ChangeTexture(name_);
	switch (renderManager->GetGraphicsMode())
	{
	case GraphicsMode::GL:
		textureSize = dynamic_cast<GLRenderManager*>(renderManager)->GetTexture(name_)->GetSize();
		break;
	case GraphicsMode::VK:
		textureSize = dynamic_cast<VKRenderManager*>(renderManager)->GetTexture(name_)->GetSize();
		break;
	}
	AddSpriteToManager();
}

void Sprite::LoadAnimation(const std::filesystem::path& spriteInfoFile, std::string name)
{
	hotSpotList.clear();
	frameTexel.clear();
	animations.clear();

	if (spriteInfoFile.extension() != ".spt")
	{
		throw std::runtime_error("Bad Filetype.  " + spriteInfoFile.generic_string() + " not a sprite info file (.spt)");
	}
	std::ifstream inFile(spriteInfoFile);

	if (inFile.is_open() == false)
	{
		throw std::runtime_error("Failed to load " + spriteInfoFile.generic_string());
	}

	std::string text;
	inFile >> text;
	//texturePtr = Engine::GetTextureManager().Load(text, true);
	//frameSize = texturePtr->GetSize();
	Engine::Instance().GetRenderManager()->LoadTexture(text, name);
	AddMeshWithTexel(name);

	inFile >> text;
	while (inFile.eof() == false)
	{
		if (text == "FrameSize")
		{
			inFile >> frameSize.x;
			inFile >> frameSize.y;
		}
		else if (text == "NumFrames")
		{
			int numFrames;
			inFile >> numFrames;
			for (int i = 0; i < numFrames; i++)
			{
				frameTexel.push_back({ frameSize.x * i, 0 });
			}
		}
		else if (text == "Frame")
		{
			int frameLocationX, frameLocationY;
			inFile >> frameLocationX;
			inFile >> frameLocationY;
			frameTexel.push_back({ static_cast<float>(frameLocationX), static_cast<float>(frameLocationY) });
		}
		else if (text == "HotSpot")
		{
			int hotSpotX, hotSpotY;
			inFile >> hotSpotX;
			inFile >> hotSpotY;
			hotSpotList.push_back({ static_cast<float>(hotSpotX), static_cast<float>(hotSpotY) });
		}
		else if (text == "Anim")
		{
			inFile >> text;
			animations.push_back(new Animation{ text });
		}
		//else if (text == "CollisionRect")
		//{
		//	rect3 rect;
		//	inFile >> rect.point1.x >> rect.point1.y >> rect.point2.x >> rect.point2.y;
		//	if (object == nullptr)
		//	{
		//		Engine::GetLogger().LogError("Trying to add collision to a nullobject");
		//	}
		//	else
		//	{
		//		object->AddGOComponent(new RectCollision(rect, object));
		//	}
		//}
		//else if (text == "CollisionCircle")
		//{
		//	double radius;
		//	inFile >> radius;
		//	if (object == nullptr)
		//	{
		//		Engine::GetLogger().LogError("Trying to add collision to a nullobject");
		//	}
		//	else
		//	{
		//		object->AddGOComponent(new CircleCollision(radius, object));
		//	}
		//}
		else
		{
			//Engine::GetLogger().LogError("Unknown spt command " + text);
		}
		inFile >> text;
	}

	if (frameTexel.empty() == true)
	{
		frameTexel.push_back({ 0,0 });
	}

	if (animations.empty())
	{
		animations.push_back(new Animation{});
		PlayAnimation(0);
	}
}

glm::vec2 Sprite::GetHotSpot(int index)
{
	if (index < 0 || hotSpotList.size() <= index)
	{
		//Engine::GetLogger().LogError("Cannot find a hotspot of current index!");
		return glm::vec2{ 0,0 };
	}
	return hotSpotList[index];
}

void Sprite::PlayAnimation(int anim)
{
	if (anim < 0 || animations.size() <= anim)
	{
		//Engine::GetLogger().LogError(std::to_string(anim) + " is out of index!");
		currAnim = 0;
	}
	else
	{
		currAnim = anim;
		animations[currAnim]->ResetAnimation();
	}
}

void Sprite::UpdateAnimation(float dt)
{
	if (animations.empty() == false && currAnim >= 0 && !animations[currAnim]->IsAnimationDone())
	{
		animations[currAnim]->Update(dt);
		Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).frameSize = glm::vec4(GetFrameSize() / textureSize, 0.f, 0.f);
		Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).texelPos = glm::vec4(GetFrameTexel(animations[currAnim]->GetDisplayFrame()) / textureSize, 0.f, 0.f);
	}
}

void Sprite::ChangeTexture(std::string name)
{
 	RenderManager* renderManager = Engine::Instance().GetRenderManager();
	switch (renderManager->GetGraphicsMode())
	{
	case GraphicsMode::GL:
	{
		GLRenderManager* renderManagerGL = dynamic_cast<GLRenderManager*>(renderManager);
		if (renderManagerGL->GetTexture(name) != nullptr)
		{
			renderManagerGL->GetFragmentVector()->at(materialId).texIndex = renderManagerGL->GetTexture(name)->GetTextrueId();
			renderManagerGL->GetVertexVector()->at(materialId).isTex = true;
		}
		else
		{
			renderManagerGL->GetVertexVector()->at(materialId).isTex = false;
		}
		break;
	}
	case GraphicsMode::VK:
	{
		VKRenderManager* renderManagerVK = dynamic_cast<VKRenderManager*>(renderManager);
		if (renderManagerVK->GetTexture(name) != nullptr)
		{
			renderManagerVK->GetFragmentVector()->at(materialId).texIndex = renderManagerVK->GetTexture(name)->GetTextrueId();
			renderManagerVK->GetVertexVector()->at(materialId).isTex = true;
		}
		else
		{
			renderManagerVK->GetVertexVector()->at(materialId).isTex = false;
		}
		break;
	}
	default:
		break;
	}
}

void Sprite::AddSpriteToManager()
{
	Engine::GetSpriteManager().AddSprite(this);
}

void Sprite::SetColor(glm::vec4 color)
{
	Engine::Instance().GetRenderManager()->GetVertexVector()->at(materialId).color = color;
}

glm::vec2 Sprite::GetFrameTexel(int frameNum) const
{
	if (frameNum < 0 || frameTexel.size() <= frameNum)
	{
		//Engine::GetLogger().LogError(std::to_string(frameNum) + " is out of index!");
		return glm::vec2{ 0,0 };
	}
	else
	{
		return frameTexel[frameNum];
	}
}
