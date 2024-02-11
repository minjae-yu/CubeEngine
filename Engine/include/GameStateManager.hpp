//Author: DOYEONG LEE
//Project: CubeEngine
//File: GameStateManager.hpp
#pragma once
#include "GameLevelType.hpp"
#include "GameState.hpp"
#include <vector>

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void LevelInit();
	void LevelInit(GameLevel currentLevel_);
	void Update(float dt);
	void Draw(float dt);
    void End();
	
	void AddLevel(GameState* level);
	void ChangeLevel(GameLevel changeLV);
	void RestartLevel();

	GameLevel GetCurrentLevel() { return currentLevel; }
private:
	GameLevel currentLevel;
	std::vector<GameState*> levelList;
};