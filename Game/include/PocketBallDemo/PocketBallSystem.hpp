//Author: DOYEONG LEE
//Project: CubeEngine
//File: PocketBallSystem.hpp
#pragma once

#include <vector>
#include "BasicComponents/Sprite.hpp"

class Engine;
class PocketBallSystem
{
public:
	PocketBallSystem(Engine* engine_) { engine = engine_; }
	~PocketBallSystem() { engine = nullptr; }

	void Init();
	void Update(float dt);
	void End();

	void Control(float dt);
	void SetBallNum(int num) { ballNum = num; }
	int GetBallNum() { return ballNum; }

	void SetPlayerBall(Object* obj);

	Engine* GetEngine() { return engine; }
private:
	Sprite* cursor = nullptr;
	Sprite* powerMeter = nullptr;

	glm::vec2 distanceMax { 0.f,0.f };
	glm::vec2 playerPosition{ 0.f,0.f };
	glm::vec2 cursorPosition{ 0.f,0.f };
	float angle = 0.f;
	float shotAngle = 0.f;

	glm::vec2 powerMeterPosition{ 0.f,0.f };
	glm::vec2 powerMeterSize{ 0.f,0.f };

	float power = 0.f;
	int ballNum = 0;
	bool isShot = false;
	bool isIncrease = true;

	Engine* engine = nullptr;
};