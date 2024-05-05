//Author: DOYEONG LEE
//Project: CubeEngine
//File: PPlayer.cpp
#include "PlatformDemo/PPlayer.hpp"
#include "PlatformDemo/PBullet.hpp"
#include "PlatformDemo/PEnemyBullet.hpp"
#include "PlatformDemo/PlatformDemoSystem.hpp"

#include "BasicComponents/Sprite.hpp"
#include "BasicComponents/Physics2D.hpp"
#include "PlatformDemo/PlatformDemoSystem.hpp"

#include "Engine.hpp"

#include <iostream>

PPlayer::PPlayer(glm::vec3 pos_, glm::vec3 size_, std::string name, ObjectType objectType, SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, InputManager* inputManager_)
	: Object(pos_, size_, name, objectType)
{
	SetManagers(spriteManager_, objectManager_, particleManager_, cameraManager_, inputManager_);
}

PPlayer::PPlayer(glm::vec3 pos_, glm::vec3 size_, std::string name, PlatformDemoSystem* sys, SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, InputManager* inputManager_)
	: Object(pos_, size_, name, ObjectType::PLAYER)
{
	SetManagers(spriteManager_, objectManager_, particleManager_, cameraManager_, inputManager_);
	AddComponent<Sprite>();
	GetComponent<Sprite>()->SetManagers(Object::spriteManager, Object::cameraManager);
	GetComponent<Sprite>()->LoadAnimation("../Game/assets/PlatformDemo/player.spt", "Player");

	AddComponent<Physics2D>();
	GetComponent<Physics2D>()->SetMinVelocity({ 0.01f, 0.1f });
	GetComponent<Physics2D>()->SetGravity(40.f);
	GetComponent<Physics2D>()->SetFriction(0.9f);
	GetComponent<Physics2D>()->SetMaxVelocity({ 10.f,20.f });
	GetComponent<Physics2D>()->AddCollidePolygonAABB({ size_.x / 2.f,  size_.y / 2.f });
	GetComponent<Physics2D>()->SetBodyType(BodyType::RIGID);
	GetComponent<Sprite>()->PlayAnimation(0);
	platformDemoSystem = sys;
}


void PPlayer::Init()
{
}

void PPlayer::Update(float dt)
{
	Object::Update(dt);
	GetComponent<Physics2D>()->Gravity(dt);
	Jumping();
	Control(dt);

	if (canAttack == false)
	{
		if (GetComponent<Sprite>()->IsAnimationDone() == true)
		{
			canAttack = true;
		}
	}

	if (isInvincible == true)
	{
		invincibleDelay += dt;
		if (invincibleDelay > 1.f)
		{
			isInvincible = false;
			invincibleDelay = 0.f;
		}
	}
}

void PPlayer::End()
{
	platformDemoSystem = nullptr;
	Object::particleManager->Clear();
	Object::objectManager->DestroyAllObjects();
}

void PPlayer::CollideObject(Object* obj)
{
	switch (obj->GetObjectType())
	{
	case ObjectType::WALL:
		GetComponent<Physics2D>()->CheckCollision(obj);
		obj->GetComponent<Physics2D>()->CheckCollision(this);
		break;
	case ObjectType::ENEMYBULLET:
		if (GetInvincibleState() == false && GetComponent<Physics2D>()->CheckCollision(obj) == true)
		{
			PEnemyBullet* b = static_cast<PEnemyBullet*>(obj);

			platformDemoSystem->HpDecrease(b->GetDamage());
			SetInvincibleState(true);

			Object::objectManager->Destroy(b->GetId());
			b = nullptr;
		}
		break;
	}
}

void PPlayer::Control(float /*dt*/)
{
	if (Object::inputManager->IsKeyPressed(KEYBOARDKEYS::DOWN))
	{
	}
	if (Object::inputManager->IsKeyPressed(KEYBOARDKEYS::UP))
	{
	}
	if (Object::inputManager->IsKeyPressed(KEYBOARDKEYS::LEFT))
	{
		if (IsStateOn(PlayerStates::DIRECTION) == true)
		{
			Object::SetXSize(-Object::GetSize().x);
		}
		SetStateOff(PlayerStates::DIRECTION);
		GetComponent<Physics2D>()->AddForceX(-20.f);
	}
	if (Object::inputManager->IsKeyPressed(KEYBOARDKEYS::RIGHT))
	{
		if (IsStateOn(PlayerStates::DIRECTION) == false)
		{
			Object::SetXSize(-Object::GetSize().x);
		}
		SetStateOn(PlayerStates::DIRECTION);
		GetComponent<Physics2D>()->AddForceX(20.f);
	}
	if (Object::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::X)
		&& IsStateOn(PlayerStates::FALLING) == false && IsStateOn(PlayerStates::JUMPING) == false)
	{
		Object::SetYPosition(Object::GetPosition().y + 1.f);
		GetComponent<Physics2D>()->SetVelocityY(40.f);
	}
	if (Object::inputManager->IsKeyPressedOnce(KEYBOARDKEYS::Z))
	{
		if (canAttack == true)
		{
			{
				GetComponent<Sprite>()->PlayAnimation(3);
				canAttack = false;
			}

			Object::objectManager->AddObject<PBullet>(position, glm::vec3{ 8.f,8.f,0.f }, "Bullet", Object::spriteManager, Object::objectManager, Object::particleManager, Object::cameraManager, Object::inputManager);
			if (IsStateOn(PlayerStates::DIRECTION))
			{
				Object::objectManager->GetLastObject()->SetXSpeed(1000.f);
			}
			else
			{
				Object::objectManager->GetLastObject()->SetXSpeed(-1000.f);
			}
		}
	}
}

void PPlayer::Jumping()
{
	if (GetComponent<Physics2D>()->GetVelocity().y > 0.f)
	{
		if (IsStateOn(PlayerStates::JUMPING) == false)
		{
			if (canAttack == true)
			{
				GetComponent<Sprite>()->PlayAnimation(2);
			}

			SetStateOn(PlayerStates::JUMPING);
			SetStateOff(PlayerStates::FALLING);
		}
	}
	else if (GetComponent<Physics2D>()->GetVelocity().y > -0.9f &&
		GetComponent<Physics2D>()->GetVelocity().y < 0.0f)
	{
		if (IsStateOn(PlayerStates::FALLING) == true)
		{
			SetStateOff(PlayerStates::FALLING);
			SetStateOff(PlayerStates::JUMPING);
			if (canAttack == true)
			{
				if (GetComponent<Physics2D>()->GetVelocity().x < 0.5f ||
					GetComponent<Physics2D>()->GetVelocity().x > -0.5f)
				{
					GetComponent<Sprite>()->PlayAnimation(0);
				}
				else
				{
					GetComponent<Sprite>()->PlayAnimation(1);
				}
			}
		}
	}
	else if (GetComponent<Physics2D>()->GetVelocity().y < 0.f)
	{
		if (canAttack == true)
		{
			GetComponent<Sprite>()->PlayAnimation(2);
		}

		if (IsStateOn(PlayerStates::FALLING) == false)
		{
			SetStateOn(PlayerStates::FALLING);
			SetStateOff(PlayerStates::JUMPING);
		}
	}
}