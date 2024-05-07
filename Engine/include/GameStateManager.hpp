//Author: DOYEONG LEE
//Project: CubeEngine
//File: GameStateManager.hpp
#pragma once
#include "GameLevelType.hpp"
#include "GameState.hpp"
#include <vector>


enum class State
{
	START,
	LOAD,
	UPDATE,
	UNLOAD,
	SHUTDOWN,
	RESTART,
	CHANGE,
};

class SpriteManager;
class ObjectManager;
class ParticleManager;
class CameraManager;
class SoundManager;
class InputManager;

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void Init(SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, SoundManager* soundManager_, InputManager* inputManager_);
	void LevelInit(GameLevel currentLevel_);
	void Update(float dt);
	void Draw();
	void LevelEnd();

	void AddLevel(GameState* level);
	void ChangeLevel(GameLevel lev);
	void RestartLevel();

	GameLevel GetCurrentLevel() { return currentLevel; }

	void SetGameState(State state_) { state = state_; }
	State GetGameState() { return state; }
	
	void End();
#ifdef _DEBUG
	void StateChanger();
	void DrawWithImGui(float dt);
#endif
private:
#ifdef _DEBUG
	const char* GameLevelTypeEnumToChar(GameLevel type);
#endif
	void LevelInit();
	void CollideObjects();

	GameLevel currentLevel = GameLevel::NONE;
	GameLevel levelSelected = GameLevel::NONE;
	std::vector<GameState*> levelList;
	State state = State::START;

	SpriteManager* spriteManager = nullptr;
	ObjectManager* objectManager = nullptr;
	ParticleManager* particleManager = nullptr;
	CameraManager* cameraManager = nullptr;
	SoundManager* soundManager = nullptr;
	InputManager* inputManager = nullptr;
};