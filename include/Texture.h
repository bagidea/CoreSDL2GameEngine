#pragma once

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture
{
public:
	static std::map<std::string, SDL_Texture*> loaders;
	Texture(const char* path);
	~Texture();
	SDL_Surface* GetImage();
	std::string GetPath();
private:
	SDL_Surface* img;
	std::string path;
};
