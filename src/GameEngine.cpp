#include "GameEngine.h"

int GameEngine::ScreenWidth = 0;
int GameEngine::ScreenHeight = 0;
Scene* Scene::Current = NULL;
std::map<std::string, SDL_Texture*> Texture::loaders;
int Mouse::mouseX = 0;
int Mouse::mouseY = 0;
int Mouse::mouseButton = 0;
int Mouse::mouseWheelX = 0;
int Mouse::mouseWheelY = 0;

GameEngine::GameEngine(const char* title, int screen_width, int screen_height)
{
	this->title = title;
	this->screen_width = GameEngine::ScreenWidth = screen_width;
	this->screen_height = GameEngine::ScreenHeight = screen_height;
	window = NULL;
	isQuit = false;
	scene = NULL;
	init();
}

GameEngine::~GameEngine()
{
	if(window != NULL)
		SDL_DestroyWindow(window);
	window = NULL;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void GameEngine::Run()
{
	Start();
	while(!isQuit)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			{
				isQuit = true;
				break;
			}
			else if(e.type == KEYDOWN || e.type == KEYUP)
			{
				KeyboardEvent(e.type, e.key.keysym.sym);
				if(scene != NULL && !isQuit)
					scene->KeyboardUpdate(e.type, e.key.keysym.sym);
			}
			else if(e.type == MOUSE_MOTION || e.type == MOUSE_BUTTONDOWN || e.type == MOUSE_BUTTONUP || MOUSE_WHEEL)
			{
				SDL_GetMouseState(&Mouse::mouseX, &Mouse::mouseY);
				if(e.type == MOUSE_BUTTONUP || e.type == MOUSE_BUTTONDOWN)
					Mouse::mouseButton = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(MOUSE_BUTTON_LEFT))?MOUSE_BUTTON_LEFT:(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(MOUSE_BUTTON_MIDDLE))?MOUSE_BUTTON_MIDDLE:(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(MOUSE_BUTTON_RIGHT))?MOUSE_BUTTON_RIGHT:(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(MOUSE_BUTTON_X1))?MOUSE_BUTTON_X1:(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(MOUSE_BUTTON_X2))?MOUSE_BUTTON_X2:0;
				if(e.type == MOUSE_WHEEL)
				{
					Mouse::mouseWheelX = e.wheel.x;
					Mouse::mouseWheelY = e.wheel.y;
				}
				MouseEvent(e.type);
				if(scene != NULL && !isQuit)
					scene->MouseUpdate(e.type);
			}
		}
		Update();
		if(scene != NULL && !isQuit)
			scene->Render();
	}
}

void GameEngine::SetScene(Scene* scene)
{
	Scene::Current = this->scene = scene;
	this->scene->Init(window);
}

int GameEngine::GetScreenWidth()
{
	return screen_width;
}

int GameEngine::GetScreenHeight()
{
	return screen_height;
}

void GameEngine::KeyboardEvent(int key_event, int key_code){}
void GameEngine::MouseEvent(int mouse_event){}

int GameEngine::init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL could not initialize! : error " << SDL_GetError() << std::endl;
		return -1;
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		std::cout << "Create window failed! : error - " << SDL_GetError() << std::endl;
		return -1;
	}
	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! : error - " << IMG_GetError() << std::endl;
		return -1;
	}
	if(TTF_Init() < 0)
	{
		std::cout << "SDL_ttf could not initialize! : error - " << TTF_GetError() << std::endl;
		return -1;
	}
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 204) < 0)
	{
		std::cout << "SDL_mixer could not initialze! : error - " << Mix_GetError() << std::endl;
		return -1;
	}
	return 0;
}

void GameEngine::Start(){}
void GameEngine::Update(){}
