#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "KeyboardAndMouse.h"
#include "Scene.h"
#include "Audio.h"

class GameEngine
{
public:
	static int ScreenWidth;
	static int ScreenHeight;
	GameEngine(std::string title, int screen_width, int screen_height);
	~GameEngine();
	void Run();
	std::shared_ptr<Scene> CreateScene();
	void SetScene(std::shared_ptr<Scene> scene);
	int GetScreenWidth();
	int GetScreenHeight();
	virtual void KeyboardEvent(int key_event, int key_code);
	virtual void MouseEvent(int mouse_event);
private:
	std::string title;
	int screen_width, screen_height;
	SDL_Window* window;
	bool isQuit;
	SDL_Event e;
	std::vector<std::shared_ptr<Scene>> scenes;
	std::shared_ptr<Scene> scene;
	int init();
	virtual void Start();
	virtual void Update();
};
