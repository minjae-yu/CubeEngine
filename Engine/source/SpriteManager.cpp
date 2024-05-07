//Author: DOYEONG LEE
//Project: CubeEngine
//File: SpriteManager.cpp
#include "SpriteManager.hpp"
#include "Engine.hpp"

#include <iostream>

SpriteManager::~SpriteManager()
{
	//if (sprites.empty() != true)
	//{
	//	auto iterator = sprites.begin();
	//	for (auto it = iterator + 1; it != sprites.end(); it++)
	//	{
	//		Engine::Instance().GetVKRenderManager().DeleteWithIndex();
	//		sprites.erase(iterator);
	//		std::cout << sprites.size() << std::endl;
	//	}
	//}
	End();
}

void SpriteManager::Update(float /*dt*/)
{
	//for (int i = 0; i < sprites.size(); i++)
	//{
	//	//sprites[i]->Update(dt);
	//	sprites[i]->Update(dt, sprites[i]->GetMaterialId());
	//}
}

void SpriteManager::End()
{
	engine = nullptr;
}

void SpriteManager::AddSprite(Sprite* sprite_)
{
	sprites.push_back(sprite_);
}

void SpriteManager::DeleteSprite(Sprite* sprite_)
{
	if (sprites.empty() != true)
	{
		auto iterator = std::find(sprites.begin(), sprites.end(), sprite_);
		for (auto it = iterator + 1; it != sprites.end(); it++)
		{
			(*it)->SetMaterialId((*it)->GetMaterialId() - 1);
			engine->GetVKRenderManager().GetVertexVector()->at((*it)->GetMaterialId()) = engine->GetVKRenderManager().GetVertexVector()->at((*it)->GetMaterialId() + 1);
			engine->GetVKRenderManager().GetFragmentVector()->at((*it)->GetMaterialId()) = engine->GetVKRenderManager().GetFragmentVector()->at((*it)->GetMaterialId() + 1);
		}
		engine->GetVKRenderManager().DeleteWithIndex();
		sprites.erase(iterator);
	}
}