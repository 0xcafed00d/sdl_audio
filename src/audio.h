#ifndef SDL_AUDIO_H
#define SDL_AUDIO_H

#include <stdint.h>
#include <stdexcept>

struct audio_exception : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

void AUDIO_Init();
int AUDIO_LoadWav(const char* name);
void AUDIO_Play(int id, int chan, bool loop);
bool AUDIO_Playing(int chan);

#endif /* SDL_AUDIO_H */
