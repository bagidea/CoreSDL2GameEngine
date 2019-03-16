#include "Sprite.h"

Sprite::Sprite(std::shared_ptr<Texture> loader)
{
	visible = true;
	x = y = pivotX = pivotY = rotation = frame = 0;
	fps = 24;
	isPlay = false;
	texture = NULL;
	path = loader->GetPath();
	img = loader->GetImage();
	if(img == NULL)
	{
		if(Texture::loaders[path] == NULL)
			std::cout << "Sprite load Texture image! : error - " << IMG_GetError() << std::endl;
		else
		{
			int w, h;
			SDL_QueryTexture(Texture::loaders[path], NULL, NULL, &w, &h);
			img_width = width = w;
			img_height = height = h;
		}
	}else{
		img_width = width = img->w;
		img_height = height = img->h;
	}
	tag = "default";
	SDL_Rect clip = {0, 0, img_width, img_height};
	clips.push_back(clip);
	std::vector<int> new_tag = {0};
	tags[tag] = new_tag;
	tagsMode[tag] = TAG_MODE_ONCE;
}

Sprite::~Sprite(){}

int Sprite::Init(SDL_Renderer* renderer)
{
	if(img != NULL)
	{
		if(texture == NULL)
		{
			if(Texture::loaders[path] == NULL)
			{
				texture = SDL_CreateTextureFromSurface(renderer, img);
				if(texture == NULL)
				{
					std::cout << "Unable to create texture! : error - " << SDL_GetError() << std::endl;
					return -1;
				}
				Texture::loaders[path] = texture;
			}else{
				texture = Texture::loaders[path];
			}
		}
	}else{
		if(Texture::loaders[path] == NULL)
		{
			std::cout << "Null! image!" << std::endl;
			return -1;
		}else{
			texture = Texture::loaders[path];
		}
	}
	Start();
	return 0;
}

void Sprite::SetVisible(bool visible)
{
	this->visible = visible;
}

bool Sprite::GetVisible()
{
	return visible;
}

void Sprite::CropImage(int x, int y, int width, int height)
{
	this->width = width;
	this->height = height;
	tag = "default";
	SDL_Rect clip = {x, y, width, height};
	clips.clear();
	clips.push_back(clip);
	tags.clear();
	std::vector<int> new_tag = {0};
	tags[tag] = new_tag;
	tagsMode.clear();
	tagsMode[tag] = TAG_MODE_ONCE;
}

void Sprite::CreateSpriteSheet(int num_x, int num_y)
{
	if(img != NULL)
	{
		width = img_width/num_x;
		height = img_height/num_y;
		for(int i = 0; i < num_y; i++)
		{
			for(int a = 0; a < num_x; a++)
			{
				SDL_Rect clip = {a*width, i*height, width, height};
				clips.push_back(clip);
			}
		}
	}
}

void Sprite::SetFPS(unsigned int fps)
{
	this->fps = fps;
}

unsigned int Sprite::GetFPS()
{
	return fps;
}

void Sprite::AddTag(std::string tag_name, int index_image)
{
	tags[tag_name].push_back(index_image);
	tagsMode[tag_name] = TAG_MODE_ONCE;
	tag = tag_name;
	frame = 0;
}

void Sprite::AddTag(std::string tag_name, int start_image, int end_image, int tag_mode)
{
	std::vector<int> new_tag;
	for(int i = start_image; i <= end_image; i++)
		new_tag.push_back(i);
	tags[tag_name] = new_tag;
	tagsMode[tag_name] = tag_mode;
	tag = tag_name;
	frame = 0;
}

void Sprite::Goto(std::string tag_name)
{
	tag = tag_name;
	frame = 0;
	isPlay = false;
}

void Sprite::GotoAndPlay(std::string tag_name)
{
	tag = tag_name;
	frame = 0;
	isPlay = true;
	tmr = SDL_GetTicks();
}

void Sprite::Play()
{
	isPlay = true;
	tmr = SDL_GetTicks();
}

void Sprite::Stop()
{
	isPlay = false;
}

bool Sprite::IsPlay()
{
	return isPlay;
}

std::string Sprite::IsTag()
{
	return tag;
}

RawSprite Sprite::GetRawSprite()
{
	Update();
	SDL_Rect clip = clips[tags[tag][frame]];
	SDL_Rect render = {(width > 0)?x-pivotX:x+pivotX+width, (height > 0)?y-pivotY:y+pivotY+height, abs(width), abs(height)};
	SDL_Point pivot = {pivotX, pivotY};
	if(isPlay)
	{
		if(SDL_GetTicks()-tmr >= 1000/fps)
		{
			frame++;
			tmr = SDL_GetTicks();
		}
	}
	if(tagsMode[tag] == TAG_MODE_LOOP)
	{
		if(frame > tags[tag].size()-1)
			frame = 0;
	}
	else if(tagsMode[tag] == TAG_MODE_ONCE)
	{
		if(frame > tags[tag].size()-1)
		{
			frame = tags[tag].size()-1;
			isPlay = false;
		}
	}
	SDL_RendererFlip flip;
	if(width < 0 && height < 0)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if(width < 0)
		flip = SDL_FLIP_HORIZONTAL;
	else if(height < 0)
		flip = SDL_FLIP_VERTICAL;
	else
		flip = SDL_FLIP_NONE;
	return RawSprite(texture, clip, render, pivot, rotation, flip);
}

void Sprite::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Sprite::GetX()
{
	return x;
}

int Sprite::GetY()
{
	return y;
}

void Sprite::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

int Sprite::GetWidth()
{
	return width;
}

int Sprite::GetHeight()
{
	return height;
}

void Sprite::SetPivot(int pivotX, int pivotY)
{
	this->pivotX = pivotX;
	this->pivotY = pivotY;
}

int Sprite::GetPivotX()
{
	return pivotX;
}

int Sprite::GetPivotY()
{
	return pivotY;
}

void Sprite::SetRotation(int rotation)
{
	this->rotation = rotation;
}

int Sprite::GetRotation()
{
	return rotation;
}

void Sprite::GeometryUpdate(int layer)
{
	if(layer == 0)
		GeometryBack();
	else
		GeometryFront();
}

void Sprite::KeyboardEvent(int key_event, int key_code){}
void Sprite::MouseEvent(int mouse_event){}
void Sprite::Start(){}
void Sprite::Update(){}
void Sprite::GeometryBack(){}
void Sprite::GeometryFront(){}
