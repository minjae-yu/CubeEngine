//Author: DOYEONG LEE
//Project: CubeEngine
//File: Window.hpp
#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"
#include "SDL2/SDL_syswm.h"

#include "glm/vec2.hpp"
#include <memory>

enum class WindowMode
{
	NORMAL,
	BORADLESS,
	NONE
};

class Window
{
public:
	Window() = default;
	~Window() = default;
	void Init(const char* title, int width, int height, bool fullscreen, WindowMode mode);

	SDL_Window* GetWindow() { return window.get(); };
	bool GetMinimized() { return isMinimized; };

	glm::vec2 GetWindowSize() { return wSize; }
private:
	//SDL_Window* window;
	struct SDLWindowDestroyer
	{
		void operator()(SDL_Window* w) const
		{
			SDL_DestroyWindow(w);
		}
	};
	std::unique_ptr<SDL_Window, SDLWindowDestroyer> window;

	glm::vec2 wSize = { 0,0 };
	bool isMinimized{ false };
};