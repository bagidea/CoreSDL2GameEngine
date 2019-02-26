#include "Loader.h"

Loader::Loader(const char* path)
{
	if(Loader::loaders[path] == NULL)
	{
		img = IMG_Load(path);
		if(img == NULL)
			std::cout << "Unable to load image! : error - " << IMG_GetError() << std::endl;
	}
	this->path = path;
}

Loader::~Loader()
{
	if(img != NULL)
		SDL_FreeSurface(img);
	img = NULL;
	if(Loader::loaders[path] != NULL)
	{
		SDL_DestroyTexture(Loader::loaders[path]);
		Loader::loaders.erase(path);
	}
}

SDL_Surface* Loader::GetImage()
{
	if(img != NULL)
		return img;
	return NULL;
}

std::string Loader::GetPath()
{
	return path;
}
