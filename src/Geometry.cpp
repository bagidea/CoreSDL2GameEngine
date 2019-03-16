#include "Geometry.h"

void Geometry::DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Geometry::DrawRect(SDL_Renderer* renderer, int x, int y, int width, int height, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderDrawRect(renderer, &rect);
}

void Geometry::DrawFillRect(SDL_Renderer* renderer, int x, int y, int width, int height, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &rect);
}
