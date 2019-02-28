#pragma once

#define TAG_MODE_ONCE 0
#define TAG_MODE_LOOP 1

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <SDL2/SDL.h>
#include "KeyboardAndMouse.h"
#include "Loader.h"

struct RawSprite
{
	SDL_Texture* texture;
	SDL_Rect clip, render;
	SDL_Point pivot;
	int rotation;
	SDL_RendererFlip flip;
	RawSprite(SDL_Texture* _texture, SDL_Rect _clip, SDL_Rect _render, SDL_Point _pivot, int _rotation, SDL_RendererFlip _flip) : texture(_texture), clip(_clip), render(_render), pivot(_pivot), rotation(_rotation), flip(_flip){}
};

class Sprite
{
public:
	Sprite(Loader* loader);
	~Sprite();
	int Init(SDL_Renderer* renderer);
	void CreateSpriteSheet(int num_x, int num_y);
	void SetFPS(unsigned int fps);
	unsigned int GetFPS();
	void AddTag(std::string tag_name, int index_image);
	void AddTag(std::string tag_name, int start_image, int end_image, int tag_mode);
	void Goto(std::string tag_name);
	void GotoAndPlay(std::string tag_name);
	void Play();
	void Stop();
	bool IsPlay();
	std::string IsTag();
	RawSprite GetRawSprite();
	void SetPosition(int x, int y);
	int GetX();
	int GetY();
	void SetSize(int width, int height);
	int GetWidth();
	int GetHeight();
	void SetPivot(int pivotX, int pivotY);
	int GetPivotX();
	int GetPivotY();
	void SetRotation(int rotation);
	int GetRotation();
	virtual void KeyboardEvent(int key_event, int key_code);
	virtual void MouseEvent(int mouse_event);
private:
	SDL_Surface* img;
	std::string path;
	SDL_Texture* texture;
	unsigned int tmr, fps;
	int x, y, img_width, img_height, width, height, pivotX, pivotY, rotation, frame;
	bool isPlay;
	std::vector<SDL_Rect> clips;
	std::string tag;
	std::map<std::string, std::vector<int>> tags;
	std::map<std::string, int> tagsMode;
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();
};
