#include "audio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <stdint.h>
#include <map>

struct Wav {
	SDL_AudioSpec spec;
	uint8_t* data;
	uint32_t datalen;
};

SDL_AudioDeviceID audioDevice = 0;

static void throw_error(std::string msg) {
	msg += SDL_GetError();
	throw(audio_exception(msg));
}

void AUDIO_Init() {
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		throw_error("SDL_Init Error: ");
	}

	SDL_AudioSpec spec, gotspec;
	SDL_zero(spec);
	spec.freq = 22050;
	spec.format = AUDIO_S16LSB;
	spec.channels = 1;
	spec.samples = 4096;
	spec.callback = nullptr;
	audioDevice = SDL_OpenAudioDevice(nullptr, 0, &spec, &gotspec, 0);
	if (audioDevice == 0) {
		throw_error("SDL_OpenAudioDevice error: ");
	}
}

int AUDIO_LoadWav(const char* name) {
	Wav wav;
	SDL_zero(wav);

	if (SDL_LoadWAV(name, &wav.spec, &wav.data, &wav.datalen) == nullptr) {
		throw_error("SDL_LoadWav error: ");
	}

	SDL_QueueAudio(audioDevice, wav.data, wav.datalen);
	SDL_PauseAudioDevice(audioDevice, 0);

	return 0;
}

void AUDIO_Play(int id) {
}
