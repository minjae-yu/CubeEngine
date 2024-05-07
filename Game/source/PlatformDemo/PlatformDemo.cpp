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
	platformDemoSystem = new PlatformDemoSystem(engine);
	platformDemoSystem->Init();

	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/train_editor.png", "train_editor");
	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building1.png", "building1");
	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building2.png", "building2");
	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/building3.png", "building3");
	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/rail.png", "rail");
	engine->GetVKRenderManager().LoadTexture("../Game/assets/PlatformDemo/TrainSide.png", "trainSide");

	platformDemoSystem->LoadLevelData("../Game/assets/PlatformDemo/Stage.txt");
	engine->GetObjectManager().AddObject<PPlayer>(glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ 64.f, 96.f,0.f }, "Player", platformDemoSystem, engine);
	engine->GetObjectManager().AddObject<PEnemy>(glm::vec3{ -64.f,196.f,0.f }, glm::vec3{ 64.f, 96.f,0.f }, "Enemy", EnemyType::NORMAL, engine);
	engine->GetObjectManager().AddObject<PEnemy>(glm::vec3{ 640.f,0.f,0.f }, glm::vec3{ 320.f, 320.f,0.f }, "Enemy", EnemyType::AIRSHIP, engine);
	platformDemoSystem->InitHealthBar();
}

void PlatformDemo::Update(float dt)
{
	if (engine->GetInputManager().IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_1))
	{
		engine->GetGameStateManager().ChangeLevel(GameLevel::POCKETBALL);
	}
	else if (engine->GetInputManager().IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_2))
	{
		engine->GetGameStateManager().ChangeLevel(GameLevel::PLATFORMDEMO);
	}
	else if (engine->GetInputManager().IsKeyPressedOnce(KEYBOARDKEYS::R))
	{
		engine->GetGameStateManager().SetGameState(State::RESTART);
	}

	platformDemoSystem->Update(dt);
}

#ifdef _DEBUG
void PlatformDemo::ImGuiDraw(float /*dt*/)
{
	ImGui::ShowDemoWindow();
	engine->GetGameStateManager().StateChanger();
	engine->GetSoundManager().MusicPlayerForImGui();
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

	engine->GetCameraManager().Reset();
	engine->GetParticleManager().Clear();
	engine->GetObjectManager().DestroyAllObjects();
}

