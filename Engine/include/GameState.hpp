//Author: DOYEONG LEE
//Second Author: JEYOON YU
//Project: CubeEngine
//File: GameState.hpp
#pragma once

class Engine;
class GameState
{
public:
	virtual ~GameState() { engine = nullptr; }

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;

	void SetEngine(Engine* engine_) { engine = engine_; }
#ifdef _DEBUG
	virtual void ImGuiDraw(float dt) = 0;
#endif
	virtual void Restart() = 0;
	virtual void End() = 0;
protected:
	Engine* engine = nullptr;
};