//Author: DOYEONG LEE
//Project: CubeEngine
//File: PlatformDemo.cpp
#include "PlatformDemo/PlatformDemo.hpp"
#include "Engine.hpp"

#include "PlatformDemo/PPlayer.hpp"
#include "PlatformDemo/PEnemy.hpp"
#include "PlatformDemo/PBullet.hpp"
#include "PlatformDemo/PEnemyBullet.hpp"

#include "BasicComponents/Sprite.hpp"
#include "BasicComponents/Physics2D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

void PlatformDemo::Init()
{
	platformDemoSystem = new PlatformDemoSystem(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	platformDemoSystem->Init();

	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/train_editor.png", "train_editor");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building1.png", "building1");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building2.png", "building2");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building3.png", "building3");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/rail.png", "rail");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/TrainSide.png", "trainSide");

	platformDemoSystem->LoadLevelData("../Game/assets/PlatformDemo/Stage.txt");
	GameState::objectManager->AddObject<PPlayer>(glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ 64.f, 96.f,0.f }, "Player", platformDemoSystem, spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<PEnemy>(glm::vec3{ -64.f,196.f,0.f }, glm::vec3{ 64.f, 96.f,0.f }, "Enemy", EnemyType::NORMAL, spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<PEnemy>(glm::vec3{ 640.f,0.f,0.f }, glm::vec3{ 320.f, 320.f,0.f }, "Enemy", EnemyType::AIRSHIP, spriteManager, objectManager, particleManager, cameraManager, inputManager);
	platformDemoSystem->InitHealthBar();
}

void PlatformDemo::Update(float dt)
{
	if (GameState::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_1))
	{
		GameState::gameStateManager->ChangeLevel(GameLevel::POCKETBALL);
	}
	else if (GameState::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_2))
	{
		GameState::gameStateManager->ChangeLevel(GameLevel::PLATFORMDEMO);
	}
	else if (GameState::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::R))
	{
		GameState::gameStateManager->SetGameState(State::RESTART);
	}

	platformDemoSystem->Update(dt);
}

#ifdef _DEBUG
void PlatformDemo::ImGuiDraw(float /*dt*/)
{
	ImGui::ShowDemoWindow();
	GameState::gameStateManager->StateChanger();
	GameState::soundManager->MusicPlayerForImGui();
	platformDemoSystem->UpdateMapEditorImGui();
}
#endif

void PlatformDemo::Restart()
{
	End();
	Init();
}

void PlatformDemo::End()
{
	platformDemoSystem->End();
	delete platformDemoSystem;
	platformDemoSystem = nullptr;

	GameState::cameraManager->Reset();
	GameState::particleManager->Clear();
	GameState::objectManager->DestroyAllObjects();
}

