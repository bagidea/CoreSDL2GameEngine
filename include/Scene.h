#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <SDL2/SDL.h>
#include "KeyboardAndMouse.h"
#include "Color.h"
#include "Sprite.h"
#include "Text.h"
#include "Audio.h"

class Scene
{
public:
	static std::shared_ptr<Scene> Current;
	Scene();
	~Scene();
	int Init(SDL_Window* window);
	void End();
	void SetBackgroundColor(Color color);
	void AddChild(std::shared_ptr<Sprite> sprite);
	void RemoveChild(std::shared_ptr<Sprite> sprite);
	void AddText(std::shared_ptr<Text> text);
	void RemoveText(std::shared_ptr<Text> text);
	std::shared_ptr<AudioMusic> LoadAudioMusic(std::string path);
	std::shared_ptr<AudioTrack> LoadAudioTrack(std::string path);
	void Render();
	void KeyboardUpdate(int key_event, int key_code);
	void MouseUpdate(int mouse_event);
	void SetCameraPosition(int x, int y);
	int GetCameraX();
	int GetCameraY();
	virtual void KeyboardEvent(int key_event, int key_code);
	virtual void MouseEvent(int mouse_event);
private:
	int cameraX, cameraY;
	SDL_Renderer* renderer;
	Color bgColor;
	std::vector<std::shared_ptr<Sprite>> sprites;
	std::vector<std::shared_ptr<Text>> texts;
	std::vector<std::shared_ptr<AudioMusic>> audio_musics;
	std::vector<std::shared_ptr<AudioTrack>> audio_tracks;
	virtual void Start();
	virtual void Update();
};
