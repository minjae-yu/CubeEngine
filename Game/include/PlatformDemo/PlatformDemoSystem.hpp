//Author: DOYEONG LEE
//Project: CubeEngine
//File: PlatformDemoSystem.hpp
#pragma once

#include <vector>
#include "BasicComponents/Sprite.hpp"
#include "Background.hpp"

struct Target
{
	Sprite* rect = nullptr;
	glm::vec2 startPos{ 0.f,0.f };
	glm::vec2 endPos{ 0.f,0.f };

	glm::vec2 size{ 0.f,0.f };
	glm::vec2 pos{ 0.f,0.f };
	std::string name = "";
	std::string spriteName = "";
	ObjectType type = ObjectType::NONE;

	BackgroundType backgroundType = BackgroundType::NORMAL;
	glm::vec2 speed{ 0.f,0.f };
	float depth = 0.f;
	bool isAnimation = false;
};

enum class EditorMode
{
	OBJMODIFIER,
	OBJCREATOR,
	WALLCREATOR,
	BACKGCREATOR
};

class PlatformDemoSystem;
class PDemoMapEditorDemo
{
public:
	PDemoMapEditorDemo() = default;
	PDemoMapEditorDemo(PlatformDemoSystem* sys, SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, InputManager* inputManager_)
	{
		spriteManager = spriteManager_;
		objectManager = objectManager_;
		particleManager = particleManager_;
		cameraManager = cameraManager_;
		inputManager = inputManager_;
		pSys = sys;
	}
	~PDemoMapEditorDemo() { pSys = nullptr; EndManagers(); }

	void LoadLevelData(const std::filesystem::path& filePath);
	void SaveLevelData(const std::filesystem::path& outFilePath);

	void EndManagers()
	{
		spriteManager = nullptr;
		objectManager = nullptr;
		particleManager = nullptr;
		cameraManager = nullptr;
		inputManager = nullptr;
	}
#ifdef _DEBUG
	void Init();
	void Update(float dt);
	void UpdateImGui();
	void End();
#endif

	bool GetIsEditorMod() { return isEditorMod; }
	void SetIsEditorMod(bool state) { isEditorMod = state; }
	void SetBackgroundManager(BackgroundManager* backGroundManager_) { bgm = backGroundManager_; }
private:
#ifdef _DEBUG
	void ObjectCreator();
	void BackgroundCreator();
	void WallCreator();
#endif

	bool isEditorMod = false;
	EditorMode mode = EditorMode::WALLCREATOR;
	std::vector<Object*> objects;
	std::vector<Object*> walls;

	Target* target = nullptr;
	bool isWallSetting = false;

	glm::vec2 gridSize = { 16.f, 16.f };
	PlatformDemoSystem* pSys = nullptr;
	BackgroundManager* bgm = nullptr;
	int backGSpriteNum = 0;
	int objectNum = 0;

	SpriteManager* spriteManager = nullptr;
	ObjectManager* objectManager = nullptr;
	ParticleManager* particleManager = nullptr;
	CameraManager* cameraManager = nullptr;
	InputManager* inputManager = nullptr;
};

class PlatformDemoSystem
{
public:
	PlatformDemoSystem(SpriteManager* spriteManager_, ObjectManager* objectManager_, ParticleManager* particleManager_, CameraManager* cameraManager_, InputManager* inputManager_)
	{
		spriteManager = spriteManager_;
		objectManager = objectManager_;
		particleManager = particleManager_;
		cameraManager = cameraManager_;
		inputManager = inputManager_;
	}
	~PlatformDemoSystem() { EndManagers(); };

	void Init();
	void Update(float dt);
	void End();

	void LoadLevelData(const std::filesystem::path& filePath) { mapEditor->LoadLevelData(filePath); };
	void SaveLevelData(const std::filesystem::path& outFilePath) { mapEditor->SaveLevelData(outFilePath); };

	void SetIsEditorMod(bool state) { mapEditor->SetIsEditorMod(state); backGroundManager->SetEditorMod(state); }
	bool GetIsEditorMod() { return mapEditor->GetIsEditorMod(); }

	void HpDecrease(float damage) { hp -= damage; }

	void InitHealthBar();

	void EndManagers()
	{
		spriteManager = nullptr;
		objectManager = nullptr;
		particleManager = nullptr;
		cameraManager = nullptr;
		inputManager = nullptr;
	}

#ifdef _DEBUG
	void UpdateMapEditor(float dt);
	void UpdateMapEditorImGui();
#endif
protected:
	BackgroundManager* backGroundManager = nullptr;
	PDemoMapEditorDemo* mapEditor = nullptr;
	Sprite* healthBar = nullptr;

	float maxHp = 100.f;
	float hp = 100.f;

	SpriteManager* spriteManager = nullptr;
	ObjectManager* objectManager = nullptr;
	ParticleManager* particleManager = nullptr;
	CameraManager* cameraManager = nullptr;
	InputManager* inputManager = nullptr;
};