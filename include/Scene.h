#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "KeyboardAndMouse.h"
#include "Color.h"
#include "Sprite.h"
#include "Text.h"

class Scene
{
public:
	static Scene* Current;
	Scene();
	~Scene();
	int Init(SDL_Window* window);
	void SetBackgroundColor(Color color);
	void AddChild(Sprite* sprite);
	void RemoveChild(Sprite* sprite);
	void AddText(Text* text);
	void RemoveText(Text* text);
	void Render();
	void KeyboardUpdate(int key_event, int key_code);
	void MouseUpdate(int mouse_event);
	void SetCameraPosition(int x, int y);
	int GetCameraX();
	int GetCameraY();
	virtual void KeyboardEvent(int key_event, int key_code);
	virtual void MouseEvent(int mouse_event);
private:
	int cameraX, cameraY;
	SDL_Renderer* renderer;
	Color bgColor;
	std::vector<Sprite*> sprites;
	std::vector<Text*> texts;
	virtual void Start();
	virtual void Update();
};
