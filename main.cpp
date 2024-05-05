//Author: DOYEONG LEE
//Project: CubeEngine
//File: main.cpp
#include "Engine.hpp"

//#include "VerticesDemo.hpp"
#include "PocketBallDemo/PocketBallDemo.hpp"
//#include "PlatformDemo/PlatformDemo.hpp"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define malloc(s) _malloc_dbg(s,_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

#undef main

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(1991827);
    //_crtBreakAlloc = 157;

    GameStateManager gameStateManager;
    InputManager inputManager;
    ObjectManager objectManager;
    CameraManager cameraManager;
    SoundManager soundManager;
    SpriteManager spriteManager;
    ParticleManager particleManager;


    Engine& engine = Engine::Instance();
    engine.Init("Vulkan Demo", 1280, 720, false, WindowMode::NORMAL);
    engine.SetFPS(FrameRate::FPS_60);

    gameStateManager.Init(&spriteManager, &objectManager, &particleManager, &cameraManager, &soundManager, &inputManager);
    soundManager.Initialize();
    cameraManager.Init({ 1280 ,720 }, CameraType::TwoDimension, 1.f);
    inputManager.Init(&cameraManager);
    particleManager.InitP(&spriteManager, &cameraManager);

    soundManager.LoadMusicFilesFromFolder(L"..\\Game\\assets\\Musics");
    soundManager.LoadSoundFilesFromFolder("../Game/assets/Sounds");

    gameStateManager.AddLevel(new PocketBallDemo);
    //gameStateManager.AddLevel(new PlatformDemo);
    gameStateManager.LevelInit(GameLevel::POCKETBALL);

    engine.Update(&inputManager, &gameStateManager);
    engine.End();

    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
}