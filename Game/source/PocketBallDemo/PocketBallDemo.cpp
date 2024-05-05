//Author: DOYEONG LEE
//Project: CubeEngine
//File: PocketBallDemo.cpp
#include "PocketBallDemo/PocketBallDemo.hpp"
#include "PocketBallDemo/Ball.hpp"
#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "BasicComponents/Sprite.hpp"
#include "BasicComponents/Physics2D.hpp"

#include <iostream>
#include <cmath>

void PocketBallDemo::Init()
{
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/White.png", "White");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/1.png", "1");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/2.png", "2");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/3.png", "3");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/4.png", "4");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/5.png", "5");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/6.png", "6");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/Table.png", "Table");
	Engine::GetVKRenderManager().LoadTexture("../Game/assets/PocketBall/Arrow.png", "Arrow");

	ballAmount = 7;
	pocketBallSystem = new PocketBallSystem();
	pocketBallSystem->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);

	GameState::objectManager->AddObject<Object>(glm::vec3{ 0.f,-38.f,-0.1f }, glm::vec3{ 368.f*2 + 32.f, 510.f + 88.f,0.f }, "Table");
	GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
	GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddMeshWithTexture("Table");
	GameState::objectManager->AddObject<Ball>(glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "White", BallType::WHITE, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	pocketBallSystem->SetPlayerBall(GameState::objectManager->GetLastObject());

	GameState::objectManager->AddObject<Ball>(glm::vec3{ -120.f,0.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "1", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<Ball>(glm::vec3{ -152.f,16.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "2", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<Ball>(glm::vec3{ -152.f,-16.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "3", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<Ball>(glm::vec3{ -184.f,32.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "4", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<Ball>(glm::vec3{ -184.f,0.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "5", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
	GameState::objectManager->AddObject<Ball>(glm::vec3{ -184.f,-32.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "6", BallType::OTHER, pocketBallSystem, spriteManager, cameraManager);
	GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);

	pocketBallSystem->Init();
	pocketBallSystem->SetBallNum(ballAmount);

	{
		glm::vec2 tempS{ 0.f,0.f };
		GameState::objectManager->AddObject<Object>(glm::vec3{ 0.f,192.f,0.f }, glm::vec3{ 640.f, 32.f,0.f }, "Wall", ObjectType::WALL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 1.f,1.f,1.f,0.f });

		tempS = { GameState::objectManager->GetLastObject()->GetSize().x / 2.f,GameState::objectManager->GetLastObject()->GetSize().y / 2.f };
		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x - 32.f, -tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x + 32.f, -tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);


		GameState::objectManager->AddObject<Object>(glm::vec3{ 0.f,-192.f,0.f }, glm::vec3{ 640.f, 32.f,0.f }, "Wall", ObjectType::WALL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 1.f,1.f,1.f,0.f });
		GameState::objectManager->GetLastObject()->SetRotate(180.f);

		tempS = { GameState::objectManager->GetLastObject()->GetSize().x / 2.f,GameState::objectManager->GetLastObject()->GetSize().y / 2.f };
		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x - 32.f, -tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x + 32.f, -tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);


		GameState::objectManager->AddObject<Object>(glm::vec3{ -336.f,0.f,0.f }, glm::vec3{ 352.f, 32.f,0.f }, "Wall", ObjectType::WALL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 1.f,1.f,1.f,0.f });

		tempS = { GameState::objectManager->GetLastObject()->GetSize().x / 2.f,GameState::objectManager->GetLastObject()->GetSize().y / 2.f };
		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x - 32.f, -tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x + 32.f, -tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);
		GameState::objectManager->GetLastObject()->SetRotate(90.f);

		GameState::objectManager->AddObject<Object>(glm::vec3{ 336.f,0.f,0.f }, glm::vec3{ 352.f, 32.f,0.f }, "Wall", ObjectType::WALL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 1.f,1.f,1.f,0.f });

		tempS = { GameState::objectManager->GetLastObject()->GetSize().x / 2.f,GameState::objectManager->GetLastObject()->GetSize().y / 2.f };
		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ tempS.x - 32.f, -tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x + 32.f, -tempS.y });
		//Engine::GetObjectManager()->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, -(tempS.y - 16.f) });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygon({ -tempS.x, tempS.y });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);
		GameState::objectManager->GetLastObject()->SetRotate(270.f);

		GameState::objectManager->AddObject<Object>(glm::vec3{ 336.f,192.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "Goal", ObjectType::GOAL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 0.f,0.f,0.f,0.5f });

		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygonAABB({ 2.f, 2.f });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);

		GameState::objectManager->AddObject<Object>(glm::vec3{ -336.f,192.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "Goal", ObjectType::GOAL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 0.f,0.f,0.f,0.5f });

		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygonAABB({ 2.f, 2.f });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);

		GameState::objectManager->AddObject<Object>(glm::vec3{ -336.f,-192.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "Goal", ObjectType::GOAL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 0.f,0.f,0.f,0.5f });

		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygonAABB({ 2.f, 2.f });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);

		GameState::objectManager->AddObject<Object>(glm::vec3{ 336.f,-192.f,0.f }, glm::vec3{ 32.f, 32.f,0.f }, "Goal", ObjectType::GOAL);
		GameState::objectManager->GetLastObject()->SetManagers(spriteManager, objectManager, particleManager, cameraManager, inputManager);
		GameState::objectManager->GetLastObject()->AddComponent<Sprite>();
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->SetManagers(spriteManager, cameraManager);
		GameState::objectManager->GetLastObject()->GetComponent<Sprite>()->AddQuad({ 0.f,0.f,0.f,0.5f });

		GameState::objectManager->GetLastObject()->AddComponent<Physics2D>();
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->AddCollidePolygonAABB({ 2.f, 2.f });
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetBodyType(BodyType::BLOCK);
		GameState::objectManager->GetLastObject()->GetComponent<Physics2D>()->SetMass(4.f);
	}
}

void PocketBallDemo::Update(float dt)
{
	if (GameState::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_1))
	{
		GameState::gameStateManager->ChangeLevel(GameLevel::POCKETBALL);
	}
	else if (GameState::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::NUMBER_2))
	{
		GameState::gameStateManager->ChangeLevel(GameLevel::PLATFORMDEMO);
	}
	pocketBallSystem->Update(dt);
}

#ifdef _DEBUG
void PocketBallDemo::ImGuiDraw(float /*dt*/)
{
	ImGui::ShowDemoWindow();
	GameState::gameStateManager->StateChanger();
	GameState::soundManager->MusicPlayerForImGui();
}
#endif

void PocketBallDemo::Restart()
{
	End();
	Init();
}

void PocketBallDemo::End()
{
	if(pocketBallSystem != nullptr)
	{
		pocketBallSystem->End();
	}

	delete pocketBallSystem;
	pocketBallSystem = nullptr;

	GameState::cameraManager->Reset();
	GameState::particleManager->Clear();
	GameState::objectManager->DestroyAllObjects();
}

