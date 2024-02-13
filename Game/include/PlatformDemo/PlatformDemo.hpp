//Author: DOYEONG LEE
//Project: CubeEngine
//File: PlatformDemo.hpp
#pragma once
#include "GameState.hpp"

#include <vector>

class PlatformDemo : public GameState
{
public:
	PlatformDemo() = default;
	~PlatformDemo() {};

	void Init() override;
	void Update(float dt) override;
#ifdef _DEBUG
	void ImGuiDraw(float dt) override;
#endif
	void Restart() override;
	void End() override;

private:
	void CollideObjects();
};