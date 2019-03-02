#include "Text.h"

Text::Text()
{
	init("");
}

Text::Text(std::string text)
{
	init(text);
}

Text::~Text()
{
	CleanFont();
	CleanTexture();
}

void Text::Init(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void Text::SetText(std::string text)
{
	this->text = text;
}

std::string Text::GetText()
{
	return text;
}

void Text::SetFont(std::string font_path)
{
	this->font_path = font_path;
}

void Text::SetSize(int size)
{
	this->size = size;
}

void Text::SetColor(Color color)
{
	this->color = color;
}

void Text::SetAlign(int align)
{
	this->align = align;
}

void Text::SetVAlign(int valign)
{
	this->valign = valign;
}

void Text::SetPivot(int x, int y)
{
	pivotX = x;
	pivotY = y;
}

int Text::GetPivotX()
{
	return pivotX;
}

int Text::GetPivotY()
{
	return pivotY;
}

void Text::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Text::GetX()
{
	return x;
}

int Text::GetY()
{
	return y;
}

void Text::SetRotation(int rotation)
{
	this->rotation = rotation;
}

int Text::GetRotation()
{
	return rotation;
}

RawSprite Text::GetRawSprite()
{
	SDL_Point pivot = {pivotX, pivotY};
	bool font_update = false;
	if(font == NULL || font_path != font_path_temp || size != size_temp)
	{
		CleanFont();
		font = TTF_OpenFont(font_path.c_str(), size);
		if(font == NULL)
			std::cout << "SDL_ttf failed to load font " << font_path << " : error - " << TTF_GetError() << std::endl;
		font_path_temp = font_path;
		size_temp = size;
		font_update = true;
	}
	if(texture == NULL || text != text_temp || (color.r != color_temp.r, color.g != color_temp.g, color.b != color_temp.b) || font_update)
	{
		if(text.size() > 0)
		{
			SDL_Color new_color = {Uint8(color.r), Uint8(color.g), Uint8(color.b)};
			SDL_Surface* img = TTF_RenderUTF8_Solid(font, text.c_str(), new_color);
			if(img == NULL)
				std::cout << "Render TTF fail! error - " << TTF_GetError() << std::endl;
			CleanTexture();
			texture = SDL_CreateTextureFromSurface(renderer, img);
			if(texture == NULL)
				std::cout << "Create texture TTF failed! error - " << SDL_GetError() << std::endl;
			SDL_Rect new_clip = {0, 0, img->w, img->h};
			clip = new_clip;
			SDL_Rect new_render = {x-pivotX+((align == TEXT_ALIGN_LEFT)?0:(align == TEXT_ALIGN_RIGHT)?-img->w:-(img->w/2)), y-pivotY+((valign == TEXT_VALIGN_TOP)?0:(valign == TEXT_VALIGN_BOTTOM)?-img->h:-(img->h/2)), img->w, img->h};
			render = new_render;
			color_temp = color;
			text_temp = text;
			SDL_FreeSurface(img);
			img = NULL;
		}else{
			CleanTexture();
		}
	}
	return RawSprite(texture, clip, render, pivot, rotation, SDL_FLIP_NONE);
}

void Text::init(std::string text)
{
	x = y = pivotX = pivotY = rotation = align = valign = 0;
	font_path = font_path_temp = "fonts/arial.ttf";
	size = size_temp = 25;
	color = color_temp = COLOR_BLACK;
	this->text = text_temp = text;
	font = NULL;
	texture = NULL;
}

void Text::CleanFont()
{
	if(font != NULL)
		TTF_CloseFont(font);
	font = NULL;
}

void Text::CleanTexture()
{
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	texture = NULL;
}
