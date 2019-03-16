#pragma once

#include <SDL2/SDL.h>
#include "Color.h"

class Geometry
{
public:
	static void DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color);
	static void DrawRect(SDL_Renderer* renderer, int x, int y, int width, int height, Color color);
	static void DrawFillRect(SDL_Renderer* renderer, int x, int y, int width, int height, Color color);

};
