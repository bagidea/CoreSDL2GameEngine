#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Sprite.h"
#include "Color.h"

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_RIGHT 1
#define TEXT_ALIGN_MIDDLE 2
#define TEXT_VALIGN_TOP 0
#define TEXT_VALIGN_BOTTOM 1
#define TEXT_VALIGN_MIDDLE 2

class Text
{
public:
	Text();
	Text(std::string text);
	~Text();
	void Init(SDL_Renderer* renderer);
	void SetText(std::string text);
	std::string GetText();
	void SetFont(std::string font_path);
	void SetSize(int size);
	void SetColor(Color color);
	void SetAlign(int align);
	void SetVAlign(int valign);
	void SetPosition(int x, int y);
	int GetX();
	int GetY();
	void SetPivot(int x, int y);
	int GetPivotX();
	int GetPivotY();
	void SetRotation(int rotation);
	int GetRotation();
	RawSprite GetRawSprite();
private:
	SDL_Renderer* renderer;
	std::string text, text_temp;
	std::string font_path, font_path_temp;
	int x, y, size, size_temp, align, valign, pivotX, pivotY, rotation;
	Color color, color_temp;
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Rect clip, render;
	void CleanFont();
	void CleanTexture();
};
