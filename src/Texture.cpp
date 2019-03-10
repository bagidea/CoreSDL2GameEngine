#include "Texture.h"

Texture::Texture(const char* path)
{
	if(Texture::loaders[path] == NULL)
	{
		img = IMG_Load(path);
		if(img == NULL)
			std::cout << "Unable to load image! : error - " << IMG_GetError() << std::endl;
	}else{
		img = NULL;
	}
	this->path = path;
}

Texture::~Texture()
{
	if(img != NULL)
		SDL_FreeSurface(img);
	img = NULL;
	if(Texture::loaders[path] != NULL)
	{
		SDL_DestroyTexture(Texture::loaders[path]);
		Texture::loaders.erase(path);
	}
}

SDL_Surface* Texture::GetImage()
{
	if(img != NULL)
		return img;
	return NULL;
}

std::string Texture::GetPath()
{
	return path;
}
