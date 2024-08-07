//Author: DOYEONG LEE
//Project: CubeEngine
//File: BEUEnemy.cpp

#include "BeatEmUpDemo/BEUEnemy.hpp"
#include "BeatEmUpDemo/BEUPlayer.hpp"
#include "BeatEmUpDemo/BEUAttackBox.hpp"

#include "BasicComponents/Sprite.hpp"
#include "BasicComponents/Physics2D.hpp"
#include "BeatEmUpDemo/BeatEmUpDemoSystem.hpp"

#include "Engine.hpp"

#include <iostream>

BEUEnemy::BEUEnemy(glm::vec3 pos_, glm::vec3 size_, std::string name, ObjectType objectType)
	: Object(pos_, size_, name, objectType)
{
}

BEUEnemy::BEUEnemy(glm::vec3 pos_, glm::vec3 size_, std::string name, BeatEmUpDemoSystem* sys)
	: Object(pos_, size_, name, ObjectType::ENEMY)
{
	AddComponent<Sprite>();
	GetComponent<Sprite>()->LoadAnimation("../Game/assets/BeatEmUpDemo/player.spt", "Player");
	GetComponent<Sprite>()->PlayAnimation(0);
	GetComponent<Sprite>()->SetColor({ 1.f,0.f,0.f,1.f });

	AddComponent<Physics2D>();
	GetComponent<Physics2D>()->SetMinVelocity({ 0.01f, 0.1f });
	GetComponent<Physics2D>()->SetMaxVelocity({ 40.f,3.f });
	GetComponent<Physics2D>()->SetGravity(10.f);
	GetComponent<Physics2D>()->SetFriction(0.9f);
	GetComponent<Physics2D>()->AddCollidePolygonAABB({ size_.x / 3.f,  size_.y / 2.f });
	GetComponent<Physics2D>()->SetBodyType(BodyType::RIGID);
	beatEmUpDemoSystem = sys;
	SetStateOn(BEUObjectStates::DIRECTION);
}


void BEUEnemy::Init()
{
}

void BEUEnemy::Update(float dt)
{
	Object::Update(dt);
	GetComponent<Physics2D>()->Gravity(dt);
	if (IsStateOn(BEUObjectStates::KNOCKBACK))
	{
		KnockBack(dt);
	}
	else
	{
		Jumping();
		if (IsStateOn(BEUObjectStates::HIT) == false)
		{
			if (static_cast<BEUPlayer*>(Engine::GetObjectManager().FindObjectWithName("Player")) != nullptr)
			{
				if (static_cast<BEUPlayer*>(Engine::GetObjectManager().FindObjectWithName("Player"))->IsStateOn(BEUObjectStates::KNOCKBACK) == false)
				{
					Moving(dt);
					Attack(dt);
				}

				if (isAttackHit == true)
				{
					glm::vec3 playerPos = Engine::GetObjectManager().FindObjectWithName("Player")->GetPosition();
					attackDelay += dt;
					if (attackDelay > 1.f)
					{
						if ((Object::position.z < playerPos.z + 0.2f && Object::position.z > playerPos.z - 0.2f) && ((Object::position.x > playerPos.x + 1.5f && Object::position.x < playerPos.x + 2.5f)
							|| (Object::position.x < playerPos.x - 1.5f && Object::position.x > playerPos.x - 2.5f)))
						{
							attackDelay = 0.f;
							isAttackHit = false;
							canAttack = true;
							SetStateOff(BEUObjectStates::ATTACK);
						}
						else
						{
							combo = 0;
							attackDelay = 0.f;
							isAttackHit = false;
							canAttack = false;
							SetStateOff(BEUObjectStates::ATTACK);
						}
					}
				}
			}
		}
		else
		{
			hitDelay += dt;
			if (hitDelay > 0.6f)
			{
				hitDelay = 0.f;
				SetStateOff(BEUObjectStates::HIT);
				if (IsStateOn(BEUObjectStates::KNOCKBACK) == false)
				{
					GetComponent<Sprite>()->PlayAnimation(0);
				}
			}
		}
	}

	if (isInvincible == true)
	{
		invincibleDelay += dt;
		if (invincibleDelay > 0.75f)
		{
			isInvincible = false;
			invincibleDelay = 0.f;
		}
	}
}

void BEUEnemy::End()
{
	beatEmUpDemoSystem = nullptr;
	Engine::GetParticleManager().Clear();
	Engine::GetObjectManager().DestroyAllObjects();
}

void BEUEnemy::CollideObject(Object* obj)
{
	switch (obj->GetObjectType())
	{
	case ObjectType::WALL:
		GetComponent<Physics2D>()->CheckCollision(obj);
		obj->GetComponent<Physics2D>()->CheckCollision(this);
		break;
	case ObjectType::BULLET:
		if (GetInvincibleState() == false && GetComponent<Physics2D>()->CheckCollision(obj) == true && (position.z < obj->GetPosition().z + 1.f && position.z > obj->GetPosition().z - 1.f))
		{
			canAttack = false;
			SetStateOff(BEUObjectStates::ATTACK);
			if (IsStateOn(BEUObjectStates::DIRECTION) == true && obj->GetPosition().x <= position.x)
			{
				SetStateOff(BEUObjectStates::DIRECTION);
				Object::SetXSize(-Object::GetSize().x);
			}
			else if (IsStateOn(BEUObjectStates::DIRECTION) == false && obj->GetPosition().x > position.x)
			{
				SetStateOn(BEUObjectStates::DIRECTION);
				Object::SetXSize(-Object::GetSize().x);
			}
			GetComponent<Sprite>()->PlayAnimation(7);

			switch (static_cast<BEUAttackBox*>(obj)->GetAttackBoxType())
			{
			case AttackBoxType::NORMAL:
				SetStateOn(BEUObjectStates::HIT);
				if (IsStateOn(BEUObjectStates::FALLING) == true || IsStateOn(BEUObjectStates::JUMPING) == true)
				{
					SetStateOn(BEUObjectStates::KNOCKBACK);
					GetComponent<Physics2D>()->SetVelocityY(1.1f);
					if (IsStateOn(BEUObjectStates::MOVE) == true)
					{
						if (IsStateOn(BEUObjectStates::DIRECTION) == true) //Right
						{
							GetComponent<Physics2D>()->AddForceX(-30.f);
						}
						else //Left
						{
							GetComponent<Physics2D>()->AddForceX(30.f);
						}
					}
				}
				//Engine::GetObjectManager().Destroy(obj->GetId());
				break;
			case AttackBoxType::FINISH:
				SetStateOn(BEUObjectStates::HIT);
				SetStateOn(BEUObjectStates::KNOCKBACK);
				if (IsStateOn(BEUObjectStates::FALLING) == true || IsStateOn(BEUObjectStates::JUMPING) == true)
				{
					GetComponent<Physics2D>()->SetVelocityY(1.1f);
					if (IsStateOn(BEUObjectStates::DIRECTION) == true) //Right
					{
						GetComponent<Physics2D>()->AddForceX(-30.f);
					}
					else //Left
					{
						GetComponent<Physics2D>()->AddForceX(30.f);
					}
				}
				else
				{
					Object::SetYPosition(Object::GetPosition().y + 1.f);
					GetComponent<Physics2D>()->SetVelocityY(1.1f);
					if (IsStateOn(BEUObjectStates::DIRECTION) == true) //Right
					{
						GetComponent<Physics2D>()->AddForceX(-30.f);
					}
					else //Left
					{
						GetComponent<Physics2D>()->AddForceX(30.f);
					}
				}
				break;
			}
		}
		break;
	}
}

void BEUEnemy::SetIsAttackHit(bool state_)
{
	isAttackHit = state_;
	if (state_ == true)
	{
		attackDelay = 0.f;
		std::cout << combo << std::endl;
		combo++;
	}
}

void BEUEnemy::Moving(float dt)
{
	//Moving
	glm::vec3 playerPos = Engine::GetObjectManager().FindObjectWithName("Player")->GetPosition();
	if (IsStateOn(BEUObjectStates::ATTACK) == false)
	{
		if (IsStateOn(BEUObjectStates::MOVE) == true)
		{
			if (GetComponent<Sprite>()->GetCurrentAnim() != 1)
			{
				GetComponent<Sprite>()->PlayAnimation(1);
			}

			if (IsStateOn(BEUObjectStates::DIRECTION) == false)
			{
				if (Object::position.x > playerPos.x && Object::position.x < playerPos.x + 1.5f)
				{
					SetXPosition(GetPosition().x + 5.f * dt);
				}
				else
					SetXPosition(GetPosition().x - 5.f * dt);
			}
			else
			{
				if (Object::position.x < playerPos.x && Object::position.x > playerPos.x - 1.5f)
				{
					SetXPosition(GetPosition().x - 5.f * dt);
				}
				else
					SetXPosition(GetPosition().x + 5.f * dt);
			}

			if (Object::position.z > playerPos.z)
			{
				SetZPosition(GetPosition().z - 7.5f * dt);
			}
			else if (Object::position.z < playerPos.z)
			{

				SetZPosition(GetPosition().z + 7.5f * dt);
			}
		}

		//SetDirection
		if (Object::position.x > playerPos.x)
		{
			if (IsStateOn(BEUObjectStates::DIRECTION) == true)
			{
				GetComponent<Sprite>()->PlayAnimation(1);
				Object::SetXSize(-Object::GetSize().x);
				SetStateOff(BEUObjectStates::DIRECTION);
			}
		}
		else
		{
			if (IsStateOn(BEUObjectStates::DIRECTION) == false)
			{
				GetComponent<Sprite>()->PlayAnimation(1);
				Object::SetXSize(-Object::GetSize().x);
				SetStateOn(BEUObjectStates::DIRECTION);
			}
		}
		//SetDirection
		if (Object::position.z > playerPos.z || Object::position.z < playerPos.z)
		{
			SetStateOn(BEUObjectStates::MOVE);
		}
		if ((Object::position.z < playerPos.z + 0.2f && Object::position.z > playerPos.z - 0.2f) && ((Object::position.x > playerPos.x + 1.5f && Object::position.x < playerPos.x + 2.5f)
			|| (Object::position.x < playerPos.x - 1.5f && Object::position.x > playerPos.x - 2.5f)))
		{
			SetStateOff(BEUObjectStates::MOVE);
			if (canAttack == false)
			{
				if (IsStateOn(BEUObjectStates::ATTACK) == false)
				{
					attackDelay = 0.f;
					canAttack = true;
				}
			}
			if (GetComponent<Sprite>()->GetCurrentAnim() != 0)
			{
				GetComponent<Sprite>()->PlayAnimation(0);
			}
		}
		else
		{
			SetStateOn(BEUObjectStates::MOVE);
		}
	}
	//Moving
}

void BEUEnemy::Attack(float dt)
{
	if (canAttack == true && IsStateOn(BEUObjectStates::ATTACK) == false)
	{
		if (IsStateOn(BEUObjectStates::FALLING) == false && IsStateOn(BEUObjectStates::JUMPING) == false)
		{
			switch (combo)
			{
			case 0:
				attackDelay += dt;
				if (attackDelay > 0.75f)
				{
					attackDelay = 0.f;
					GetComponent<Sprite>()->PlayAnimation(3);
					canAttack = false;
					SetStateOn(BEUObjectStates::ATTACK);
					if (IsStateOn(BEUObjectStates::DIRECTION) == true)
					{
						Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ 1.f, 0.5f , 0.f }
						, glm::vec3{ 1.5f, 1.f, 0.f }, "AttackBox", this, 0.1f, AttackBoxType::NORMAL, ObjectType::ENEMYBULLET);
					}
					else
					{
						Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ -1.f, 0.5f , 0.f }
						, glm::vec3{ 1.5f, 1.f, 0.f }, "AttackBox", this, 0.1f, AttackBoxType::NORMAL, ObjectType::ENEMYBULLET);
					}
				}
				break;
			case 1:
				GetComponent<Sprite>()->PlayAnimation(3);
				canAttack = false;
				SetStateOn(BEUObjectStates::ATTACK);
				if (IsStateOn(BEUObjectStates::DIRECTION) == true)
				{
					Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ 1.f, 0.5f , 0.f }
					, glm::vec3{ 1.5f, 1.f, 0.f }, "AttackBox", this, 0.1f, AttackBoxType::NORMAL, ObjectType::ENEMYBULLET);
				}
				else
				{
					Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ -1.f, 0.5f , 0.f }
					, glm::vec3{ 1.5f, 1.f, 0.f }, "AttackBox", this, 0.1f, AttackBoxType::NORMAL, ObjectType::ENEMYBULLET);
				}
				break;
			case 2:
				GetComponent<Sprite>()->PlayAnimation(5);
				canAttack = false;
				SetStateOn(BEUObjectStates::ATTACK);
				if (IsStateOn(BEUObjectStates::DIRECTION) == true)
				{
					Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ 1.25f, 1.f , 0.f }
					, glm::vec3{ 1.f, 2.f, 0.f }, "AttackBox", this, 0.25f, AttackBoxType::FINISH, ObjectType::ENEMYBULLET);
				}
				else
				{
					Engine::GetObjectManager().AddObject<BEUAttackBox>(glm::vec3{ -1.25f, 1.f , 0.f }
					, glm::vec3{ 1.f, 2.f, 0.f }, "AttackBox", this, 0.2f, AttackBoxType::FINISH, ObjectType::ENEMYBULLET);
				}
				break;
			}
		}
	}
	else if (canAttack == false && IsStateOn(BEUObjectStates::ATTACK) == true)
	{
		if (GetComponent<Sprite>()->IsAnimationDone() == true)
		{
			//canAttack = true;
			//SetStateOff(BEUObjectStates::ATTACK);
			GetComponent<Sprite>()->PlayAnimation(0);
		}
		glm::vec3 playerPos = Engine::GetObjectManager().FindObjectWithName("Player")->GetPosition();
		if (!((Object::position.z < playerPos.z + 0.2f && Object::position.z > playerPos.z - 0.2f) && ((Object::position.x > playerPos.x + 1.5f && Object::position.x < playerPos.x + 2.5f)
			|| (Object::position.x < playerPos.x - 1.5f && Object::position.x > playerPos.x - 2.5f))))
		{
			attackDelay = 0.f;
			combo = 0;
			isAttackHit = false;
			SetStateOff(BEUObjectStates::ATTACK);
		}
	}
}

void BEUEnemy::Jumping()
{
	if (GetComponent<Physics2D>()->GetVelocity().y > 0.f)
	{
		if (IsStateOn(BEUObjectStates::JUMPING) == false)
		{
			if (canAttack == true)
			{
				GetComponent<Sprite>()->PlayAnimation(2);
			}
			else
			{
				GetComponent<Sprite>()->PlayAnimation(6);
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
			if (GetComponent<Physics2D>()->GetVelocity().x < 0.5f ||
				GetComponent<Physics2D>()->GetVelocity().x > -0.5f)
			{
				if (IsStateOn(BEUObjectStates::MOVE) == true)
				{
					GetComponent<Sprite>()->PlayAnimation(1);
				}
				else
				{
					GetComponent<Sprite>()->PlayAnimation(0);
				}
			}
			else
			{
				GetComponent<Sprite>()->PlayAnimation(0);
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

void BEUEnemy::KnockBack(float dt)
{
	if (GetComponent<Physics2D>()->GetVelocity().y > 0.f)
	{
		if (IsStateOn(BEUObjectStates::JUMPING) == false)
		{
			GetComponent<Sprite>()->PlayAnimation(8);
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
			SetStateOn(BEUObjectStates::LAYING);
			GetComponent<Sprite>()->PlayAnimation(9);

			SetYPosition(0.f);
			if (GetComponent<Physics2D>()->GetVelocity().x < 0.5f ||
				GetComponent<Physics2D>()->GetVelocity().x > -0.5f)
			{

			}
		}
		if (IsStateOn(BEUObjectStates::LAYING) == true)
		{
			layingDelay += dt;
			if (layingDelay > 1.25f)
			{
				layingDelay = 0.f;
				hitDelay = 0.f;
				SetStateOff(BEUObjectStates::HIT);
				SetStateOff(BEUObjectStates::KNOCKBACK);
				SetStateOff(BEUObjectStates::LAYING);
				GetComponent<Sprite>()->PlayAnimation(0);
			}
		}
	}
	else if (GetComponent<Physics2D>()->GetVelocity().y < 0.f)
	{
		if (IsStateOn(BEUObjectStates::FALLING) == false)
		{
			GetComponent<Sprite>()->PlayAnimation(8);
			SetStateOn(BEUObjectStates::FALLING);
			SetStateOff(BEUObjectStates::JUMPING);
		}
	}

	if (GetPosition().y <= 0.f)
	{
		GetComponent<Physics2D>()->SetVelocityY(0.f);
	}
}
