#include "audio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <stdint.h>
#include <array>
#include <vector>

static const int NUM_CHANNELS = 4;

struct Wav {
	SDL_AudioSpec spec;
	uint8_t* data;
	uint32_t datalen;
};

std::vector<Wav> loadedWavs;

SDL_AudioDeviceID audioDevice = 0;

struct ChanInfo {
	Wav* wav = nullptr;
	uint32_t current = 0;
	bool loop = false;
	bool playing = false;
};

std::array<ChanInfo, NUM_CHANNELS> channels;

static void throw_error(std::string msg) {
	msg += SDL_GetError();
	throw(audio_exception(msg));
}

static void callback(void* userdata, uint8_t* stream, int len) {
	SDL_memset(stream, 0, len);
	puts("callback");
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
	spec.callback = callback;
	audioDevice = SDL_OpenAudioDevice(nullptr, 0, &spec, &gotspec, 0);
	if (audioDevice == 0) {
		throw_error("SDL_OpenAudioDevice error: ");
	}
	SDL_PauseAudioDevice(audioDevice, 0);
}

int AUDIO_LoadWav(const char* name) {
	Wav wav;
	SDL_zero(wav);

	if (SDL_LoadWAV(name, &wav.spec, &wav.data, &wav.datalen) == nullptr) {
		throw_error("SDL_LoadWav error: ");
	}

	SDL_LockAudioDevice(audioDevice);
	loadedWavs.push_back(wav);
	SDL_UnlockAudioDevice(audioDevice);

	return 0;
}

void AUDIO_Play(int id, int chan, bool loop) {
	SDL_LockAudioDevice(audioDevice);
	ChanInfo ci;
	ci.wav = &loadedWavs[id];
	ci.loop = loop;
	ci.playing = true;
	channels[chan] = ci;
	SDL_UnlockAudioDevice(audioDevice);
}
