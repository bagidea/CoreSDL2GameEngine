#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "KeyboardAndMouse.h"
#include "Scene.h"

class GameEngine
{
public:
	static int ScreenWidth;
	static int ScreenHeight;
	GameEngine(const char* title, int screen_width, int screen_height);
	~GameEngine();
	int Init();
	void Run();
	void SetScene(Scene* scene);
	int GetScreenWidth();
	int GetScreenHeight();
	virtual void KeyboardEvent(int key_event, int key_code);
	virtual void MouseEvent(int mouse_event);
private:
	const char* title;
	int screen_width, screen_height;
	SDL_Window* window;
	bool isQuit;
	SDL_Event e;
	Scene* scene;
	virtual void Start();
	virtual void Update();
};
