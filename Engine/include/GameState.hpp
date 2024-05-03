//Author: DOYEONG LEE
//Second Author: JEYOON YU
//Project: CubeEngine
//File: GameState.hpp
#pragma once

class GameStateManager;
class SpriteManager;
class ObjectManager;
class ParticleManager;
class CameraManager;
class InputManager;
class SoundManager;

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;

	void SetManagers(SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, InputManager* inputManager_, SoundManager* soundManager_, GameStateManager* gameStateManager_)
	{
		spriteManager = spriteManager_;
		objectManager = objectManager_;
		particleManager = particleManager_;
		cameraManager = cameraManager_;
		inputManager = inputManager_;
		soundManager = soundManager_;
		gameStateManager = gameStateManager_;
	}
	void EndManagers()
	{
		spriteManager = nullptr;
		objectManager = nullptr;
		particleManager = nullptr;
		cameraManager = nullptr;
		inputManager = nullptr;
		soundManager = nullptr;
		gameStateManager = nullptr;
	}

#ifdef _DEBUG
	virtual void ImGuiDraw(float dt) = 0;
#endif
	virtual void Restart() = 0;
	virtual void End() = 0;
protected:
	SpriteManager* spriteManager = nullptr;
	ObjectManager* objectManager = nullptr;
	ParticleManager* particleManager = nullptr;
	CameraManager* cameraManager = nullptr;
	InputManager* inputManager = nullptr;
	SoundManager* soundManager = nullptr;
	GameStateManager* gameStateManager = nullptr;
};

