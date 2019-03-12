#pragma once

#include <iostream>
#include <SDL2/SDL_mixer.h>

//volume 0 to 128
#define AUDIO_MAX_VOLUME MIX_MAX_VOLUME
#define AUDIO_LOOP -1

class AudioTrack
{
public:
	AudioTrack(std::string path);
	~AudioTrack();
	void SetVolume(int volume);
	void Play();
	void Play(int loop_count);
private:
	Mix_Chunk* audio;
	void play(int loop_count);
};

class AudioMusic
{
public:
	static void SetVolume(int volume);
	AudioMusic(std::string path);
	~AudioMusic();
	void Play();
	void Pause();
	void Stop();
private:
	Mix_Music* audio;
};
