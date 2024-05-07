//Author: DOYEONG LEE
//Project: CubeEngine
//File: Engine.hpp
#pragma once
#include "Timer.hpp"
#include "Window.hpp"
#include "GameStateManager.hpp"
#include "VKInit.hpp"
#include "VKRenderManager.hpp"
#include "InputManager.hpp"
#include "ObjectManager.hpp"
#include "CameraManager.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "Particle/ParticleManager.hpp"

class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	//Engine& Instance() { return *this; }
	Window& GetWindow() { return window; }
	VKInit& GetVKInit() { return vkInit; }
	VKRenderManager& GetVKRenderManager() { return vkRenderManager; }
	GameStateManager& GetGameStateManager() { return gameStateManger; }
	InputManager& GetInputManager() { return inputManager; }
	ObjectManager& GetObjectManager() { return objectManager; }
	CameraManager& GetCameraManager() { return cameraManager; }
	SoundManager& GetSoundManager() { return soundManager; }
	SpriteManager& GetSpriteManager() { return spriteManager; }
	ParticleManager& GetParticleManager() { return particleManager; }

	void Init(const char* title, int windowWidth, int windowHeight, bool fullScreen, WindowMode mode);
	void Update();
	void End();

	void SetFPS(FrameRate fps);
private:
	Timer timer;
	std::chrono::system_clock::time_point lastTick;
	std::chrono::system_clock::time_point fpsCalcTime;

	float deltaTime = 0.f;
	int frameCount = 0;
	std::string windowTitleWithFrameCount;

	Window window;
	VKInit vkInit;
	VKRenderManager vkRenderManager;
	GameStateManager gameStateManger;
	InputManager inputManager;
	ObjectManager objectManager;
	CameraManager cameraManager;
	SoundManager soundManager;
	SpriteManager spriteManager;
	ParticleManager particleManager;
};