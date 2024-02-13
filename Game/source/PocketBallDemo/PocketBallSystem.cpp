//Author: DOYEONG LEE
//Project: CubeEngine
//File: PocketBallSystem.cpp
#include "PocketBallDemo/PocketBallSystem.hpp"
#include "Engine.hpp"

#include <iostream>

void PocketBallSystem::Init()
{
	cursor = new Sprite();
	cursor->AddQuad({ 0.f,0.f,0.f,1.f });

	powerMeter = new Sprite();
	powerMeter->AddQuad({ 1.f,0.f,0.f,1.f });

	playerPosition = glm::vec2(Engine::Instance().GetObjectManager()->FindObjectWithName("P")->GetPosition().x, Engine::Instance().GetObjectManager()->FindObjectWithName("P")->GetPosition().y);
	cursorPosition = playerPosition;
	distanceMax = { 0.f, 0.f };
}

void PocketBallSystem::Update(float dt)
{
	{
		if (isShot == false)
		{
			if (isIncrease == true)
			{
				power += 1000.f * dt;
			}
			else
			{
				power -= 1000.f * dt;
			}

			if (power > 1000.f)
			{
				isIncrease = false;
				power = 1000.f;
			}
			else if (power < 0.f)
			{
				isIncrease = true;
				power = 0.f;
			}
		}
	}
	cursor->UpdateModel({ cursorPosition.x, cursorPosition.y, 0.f }, { 32.f,32.f,0.f }, shotAngle * 60.f);
	cursor->UpdateProjection();
	cursor->UpdateView();

	powerMeter->UpdateModel({ -640.f, -360.f, 0.f }, { 640.f * (1.f / 1000.f * power),128.f,0.f }, 0.f);
	powerMeter->UpdateProjection();
	powerMeter->UpdateView();

	playerPosition = glm::vec2(Engine::Instance().GetObjectManager()->FindObjectWithName("P")->GetPosition().x, Engine::Instance().GetObjectManager()->FindObjectWithName("P")->GetPosition().y);
	shotAngle = std::atan2(playerPosition.y - cursorPosition.y, playerPosition.x - cursorPosition.x);
	if (shotAngle * 60.f > 180.f || shotAngle * 60.f < -180.f)
	{
		shotAngle = 180.f;
	}

	if (isShot == false)
	{
		cursor->SetColor({ 0.f,0.f,0.f,1.f });
	}
	else
	{
		cursor->SetColor({ 0.f,0.f,0.f,0.f });
	}

	int ballStopNum = 0;
	if (isShot == true)
	{
		for (auto& obj : Engine::Instance().GetObjectManager()->GetObjectMap())
		{
			if (obj.second->GetObjectType() == ObjectType::BALL)
			{
				if (obj.second->GetComponent<Physics2D>()->GetVelocity().x == 0.f && obj.second->GetComponent<Physics2D>()->GetVelocity().y == 0.f)
				{
					ballStopNum++;
				}
			}
			else
			{
				if (ballStopNum == ballNum)
				{
					distanceMax = { 0.f, 0.f };
					cursorPosition = { playerPosition.x + ((distanceMax.x) * cos(angle)) - ((distanceMax.y) * sin(angle)) * dt, playerPosition.y + ((distanceMax.x) * sin(angle)) + ((distanceMax.y) * cos(angle)) * dt };
					isShot = false;
					power = 0.f;
				}
				break;
			}
		}
	}
	Control(dt);
}

void PocketBallSystem::End()
{
	delete cursor;
	delete powerMeter;
}

void PocketBallSystem::Control(float dt)
{
	if (isShot == false)
	{
		if (Engine::Instance().GetInputManager()->IsKeyPressed(KEYBOARDKEYS::DOWN))
		{
			if (distanceMax.x >= 0.f)
			{
				distanceMax -= 150.f * dt;
				if (distanceMax.x < 0.f)
				{
					distanceMax = { 0.f, 0.f };
				}
				cursorPosition = { playerPosition.x + ((distanceMax.x) * cos(angle)) - ((distanceMax.y) * sin(angle)) * dt, playerPosition.y + ((distanceMax.x) * sin(angle)) + ((distanceMax.y) * cos(angle)) * dt };
			}
		}
		if (Engine::Instance().GetInputManager()->IsKeyPressed(KEYBOARDKEYS::UP))
		{
			distanceMax += 150.f * dt;
			cursorPosition = { playerPosition.x + ((distanceMax.x) * cos(angle)) - ((distanceMax.y) * sin(angle)) * dt, playerPosition.y + ((distanceMax.x) * sin(angle)) + ((distanceMax.y) * cos(angle)) * dt };
		}
		if (Engine::Instance().GetInputManager()->IsKeyPressed(KEYBOARDKEYS::LEFT))
		{
			angle -= 1.5f * dt;
			cursorPosition = { playerPosition.x + ((distanceMax.x) * cos(angle)) - ((distanceMax.y) * sin(angle)) * dt, playerPosition.y + ((distanceMax.x) * sin(angle)) + ((distanceMax.y) * cos(angle)) * dt };
		}
		if (Engine::Instance().GetInputManager()->IsKeyPressed(KEYBOARDKEYS::RIGHT))
		{
			angle += 1.5f * dt;
			cursorPosition = { playerPosition.x + ((distanceMax.x) * cos(angle)) - ((distanceMax.y) * sin(angle)) * dt, playerPosition.y + ((distanceMax.x) * sin(angle)) + ((distanceMax.y) * cos(angle)) * dt };
		}
		if (Engine::Instance().GetInputManager()->IsKeyPressedOnce(KEYBOARDKEYS::SPACE))
		{
			Engine::Instance().GetObjectManager()->FindObjectWithName("P")->GetComponent<Physics2D>()->AddForce({ power * -cos(shotAngle), power * -sin(shotAngle) });
			isShot = true;
		}
	}
}