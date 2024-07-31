//Author: DOYEONG LEE
//Project: CubeEngine
//File: BEUObject.cpp

#include "BeatEmUpDemo/BEUObject.hpp"

#include "BasicComponents/Sprite.hpp"
#include "BasicComponents/Physics2D.hpp"
#include "BeatEmUpDemo/BeatEmUpDemoSystem.hpp"

#include "Engine.hpp"

#include <iostream>

BEUObject::BEUObject(glm::vec3 pos_, glm::vec3 size_, std::string name, ObjectType objectType)
	: Object(pos_, size_, name, objectType)
{
}

BEUObject::BEUObject(glm::vec3 pos_, glm::vec3 size_, std::string name, BeatEmUpDemoSystem* sys)
	: Object(pos_, size_, name, ObjectType::NONE)
{
	AddComponent<Sprite>();
	GetComponent<Sprite>()->LoadAnimation("../Game/assets/PlatformDemo/player.spt", "Player");
	GetComponent<Sprite>()->PlayAnimation(0);

	AddComponent<Physics2D>();
	GetComponent<Physics2D>()->SetMinVelocity({ 0.01f, 0.1f });
	GetComponent<Physics2D>()->SetGravity(15.f);
	GetComponent<Physics2D>()->SetFriction(0.9f);
	GetComponent<Physics2D>()->SetMaxVelocity({ 20.f,5.f });
	GetComponent<Physics2D>()->AddCollidePolygonAABB({ size_.x / 2.f,  size_.y / 2.f });
	GetComponent<Physics2D>()->SetBodyType(BodyType::RIGID);
	beatEmUpDemoSystem = sys;
}


void BEUObject::Init()
{
}

void BEUObject::Update(float dt)
{
	Object::Update(dt);
	GetComponent<Physics2D>()->Gravity(dt);
	Jumping();
	Control(dt);

	/*if (canAttack == false)
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
	}*/
}

void BEUObject::End()
{
	beatEmUpDemoSystem = nullptr;
	Engine::GetParticleManager().Clear();
	Engine::GetObjectManager().DestroyAllObjects();
}

void BEUObject::CollideObject(Object* obj)
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
			/*PEnemyBullet* b = static_cast<PEnemyBullet*>(obj);

			beatEmUpDemoSystem->HpDecrease(b->GetDamage());
			SetInvincibleState(true);

			Engine::GetObjectManager().Destroy(b->GetId());
			b = nullptr;*/
		}
		break;
	}
}

void BEUObject::Control(float dt)
{
	if (Engine::GetInputManager().IsKeyPressed(KEYBOARDKEYS::DOWN)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		SetZPosition(GetPosition().z + 10.f * dt);
	}
	if (Engine::GetInputManager().IsKeyPressed(KEYBOARDKEYS::UP)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		SetZPosition(GetPosition().z - 10.f * dt);
	}
	if (Engine::GetInputManager().IsKeyPressed(KEYBOARDKEYS::LEFT)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		if (IsStateOn(BEUObjectStates::DIRECTION) == true)
		{
			Object::SetXSize(-Object::GetSize().x);
		}
		SetStateOff(BEUObjectStates::DIRECTION);
		SetXPosition(GetPosition().x - 10.f * dt);
		SetStateOn(BEUObjectStates::MOVE);
	}
	else if (!Engine::GetInputManager().IsKeyPressOnce(KEYBOARDKEYS::LEFT)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		std::cout << "Left" << std::endl;
		SetStateOff(BEUObjectStates::MOVE);
	}
	if (Engine::GetInputManager().IsKeyPressed(KEYBOARDKEYS::RIGHT)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		if (IsStateOn(BEUObjectStates::DIRECTION) == false)
		{
			Object::SetXSize(-Object::GetSize().x);
		}
		SetStateOn(BEUObjectStates::DIRECTION);
		SetXPosition(GetPosition().x + 10.f * dt);
		SetStateOn(BEUObjectStates::MOVE);
	}
	else if (!Engine::GetInputManager().IsKeyPressOnce(KEYBOARDKEYS::RIGHT)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		std::cout << "Right" << std::endl;
		SetStateOff(BEUObjectStates::MOVE);
	}
	if (Engine::GetInputManager().IsKeyPressOnce(KEYBOARDKEYS::X)
		&& IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
	{
		Object::SetYPosition(Object::GetPosition().y + 1.f);
		GetComponent<Physics2D>()->SetVelocityY(2.5f);
		if (IsStateOn(BEUObjectStates::MOVE) == true)
		{
			if (IsStateOn(BEUObjectStates::DIRECTION) == true) //Right
			{
				GetComponent<Physics2D>()->AddForceX(20.f);
			}
			else //Left
			{
				GetComponent<Physics2D>()->AddForceX(-20.f);
			}
		}
	}
	if (Engine::GetInputManager().IsKeyPressOnce(KEYBOARDKEYS::Z))
	{
		if (canAttack == true)
		{
			{
				GetComponent<Sprite>()->PlayAnimation(3);
				canAttack = false;
			}

			//Engine::GetObjectManager().AddObject<PBullet>(position, glm::vec3{ 8.f,8.f,0.f }, "Bullet");
			if (IsStateOn(BEUObjectStates::DIRECTION))
			{
				Engine::GetObjectManager().GetLastObject()->SetXSpeed(1000.f);
			}
			else
			{
				Engine::GetObjectManager().GetLastObject()->SetXSpeed(-1000.f);
			}
		}
	}
}

void BEUObject::Jumping()
{
	if (GetComponent<Physics2D>()->GetVelocity().y > 0.f)
	{
		if (IsStateOn(BEUObjectStates::JUMPING) == false)
		{
			if (canAttack == true)
			{
				GetComponent<Sprite>()->PlayAnimation(2);
			}

			SetStateOn(BEUObjectStates::JUMPING);
			SetStateOff(BEUObjectStates::FALLING);
		}
	}
	else if (GetComponent<Physics2D>()->GetVelocity().y > -0.9f &&
		GetComponent<Physics2D>()->GetVelocity().y < 0.0f)
	{
		if (IsStateOn(BEUObjectStates::FALLING) == true)
		{
			SetStateOff(BEUObjectStates::FALLING);
			SetStateOff(BEUObjectStates::JUMPING);
			SetYPosition(0.f);
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

		if (IsStateOn(BEUObjectStates::FALLING) == false)
		{
			SetStateOn(BEUObjectStates::FALLING);
			SetStateOff(BEUObjectStates::JUMPING);
		}
	}

	if (GetPosition().y <= 0.f)
	{
		GetComponent<Physics2D>()->SetVelocityY(0.f);
	}
}