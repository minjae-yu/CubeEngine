//Author: DOYEONG LEE
//Project: CubeEngine
//File: SpriteManager.hpp
#pragma once
#include "BasicComponents/Sprite.hpp"

#include <vector>
#include <iostream>

class Engine;
class SpriteManager
{
public:
    SpriteManager()  = default;
    ~SpriteManager();

	void Update(float dt);
    void End();

    void AddSprite(Sprite* sprite_);
    void DeleteSprite(Sprite* sprite_);

    void SetEngine(Engine* engine_) { engine = engine_; }
private:
    std::vector<Sprite*>  sprites;

    Engine* engine = nullptr;
};