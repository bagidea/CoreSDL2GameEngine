#include "Scene.h"

Scene::Scene()
{
	cameraX = cameraY = 0;
	renderer = NULL;
	bgColor = Color(0, 0, 0);
}

Scene::~Scene()
{
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	renderer = NULL;
}

int Scene::Init(SDL_Window* window)
{
	if(renderer == NULL)
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(renderer == NULL)
		{
			std::cout << "Create scene failed! : error - " << std::endl;
			return -1;
		}
	}
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
	Start();
	return 0;
}

void Scene::SetBackgroundColor(Color color)
{
	bgColor = color;
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
}

void Scene::AddChild(Sprite* sprite)
{
	if(sprite != NULL)
	{
		sprites.push_back(sprite);
		sprite->Init(renderer);
	}
}

void Scene::RemoveChild(Sprite* sprite)
{
	bool success = false;
	for(int i = 0; i < sprites.size(); i++)
	{
		if(sprites[i] == sprite)
		{
			sprites.erase(sprites.begin()+i);
			success = true;
			break;
		}
		if(success)
			break;
	}
}

void Scene::AddText(Text* text)
{
	if(text != NULL)
	{
		texts.push_back(text);
		text->Init(renderer);
	}
}

void Scene::RemoveText(Text* text)
{
	bool success = false;
	for(int i = 0; i < texts.size(); i++)
	{
		if(texts[i] == text)
		{
			texts.erase(texts.begin()+i);
			success = true;
			break;
		}
		if(success)
			break;
	}
}

void Scene::Render()
{
	if(renderer != NULL)
	{
		Update();
		SDL_RenderClear(renderer);
		for(Sprite* sp : sprites)
		{
			RawSprite rawSp = sp->GetRawSprite();
			SDL_Rect render = {rawSp.render.x-cameraX, rawSp.render.y-cameraY, rawSp.render.w, rawSp.render.h};
			SDL_RenderCopyEx(renderer, rawSp.texture, &rawSp.clip, &render, rawSp.rotation, &rawSp.pivot, rawSp.flip);
		}
		for(Text* txt : texts)
		{
			RawSprite rawTxt = txt->GetRawSprite();
			if(rawTxt.texture != NULL)
			{
				SDL_Rect render = {rawTxt.render.x-cameraX, rawTxt.render.y-cameraY, rawTxt.render.w, rawTxt.render.h};
				SDL_RenderCopyEx(renderer, rawTxt.texture, &rawTxt.clip, &render, rawTxt.rotation, &rawTxt.pivot, rawTxt.flip);
			}
		}
		SDL_RenderPresent(renderer);
	}
}

void Scene::KeyboardUpdate(int key_event, int key_code)
{
	KeyboardEvent(key_event, key_code);
	for(Sprite* sp : sprites)
		sp->KeyboardEvent(key_event, key_code);
}

void Scene::MouseUpdate(int mouse_event)
{
	MouseEvent(mouse_event);
	for(Sprite* sp : sprites)
		sp->MouseEvent(mouse_event);
}

void Scene::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

int Scene::GetCameraX()
{
	return cameraX;
}

int Scene::GetCameraY()
{
	return cameraY;
}


void Scene::KeyboardEvent(int key_event, int key_code){}
void Scene::MouseEvent(int mouse_event){}
void Scene::Start(){}
void Scene::Update(){}
