#include "Audio.h"

AudioTrack::AudioTrack(std::string path)
{
	audio = Mix_LoadWAV(path.c_str());
	if(audio == NULL)
		std::cout << "Failed to load " << path << " : error - " << Mix_GetError() << std::endl;
}

AudioTrack::~AudioTrack(){}

void AudioTrack::SetVolume(int volume)
{
	if(audio != NULL)
		Mix_VolumeChunk(audio, volume);
}

void AudioTrack::Play()
{
	play(0);
}

void AudioTrack::Play(int loop_count)
{
	play(loop_count);
}

void AudioTrack::play(int loop_count)
{
	if(audio != NULL)
		Mix_PlayChannel(-1, audio, loop_count);
}

void AudioTrack::End()
{
	if(audio != NULL)
		Mix_FreeChunk(audio);
	audio = NULL;
}

void AudioMusic::SetVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

AudioMusic::AudioMusic(std::string path)
{
	audio = Mix_LoadMUS(path.c_str());
	if(audio == NULL)
		std::cout << "Failed to music load " << path << " : error - " << Mix_GetError() << std::endl;
}

AudioMusic::~AudioMusic(){}

void AudioMusic::Play()
{
	if(Mix_PlayingMusic() == 0)
		Mix_PlayMusic(audio, -1);
	else
		Mix_ResumeMusic();
}

void AudioMusic::Pause()
{
	Mix_PauseMusic();
}

void AudioMusic::Stop()
{
	Mix_HaltMusic();
}

void AudioMusic::End()
{
	if(audio != NULL)
		Mix_FreeMusic(audio);
	audio = NULL;
}
