#include "audio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <stdint.h>
#include <algorithm>
#include <array>
#include <vector>

static const int NUM_CHANNELS = 4;

struct Wav {
	SDL_AudioSpec spec;
	int16_t* sampleData;
	uint32_t numSamples;
};

std::vector<Wav> loadedWavs;

SDL_AudioDeviceID audioDevice = 0;

struct Channel {
	Wav* wav = nullptr;
	uint32_t current = 0;
	bool loop = false;
	bool playing = false;
};

inline int16_t getNextSample(Channel& c) {
	if (!c.playing)
		return 0;

	if (c.current == c.wav->numSamples) {
		if (c.loop) {
			c.current = 0;
		} else {
			c.playing = false;
			return 0;
		}
	}
	return c.wav->sampleData[c.current++];
}

std::array<Channel, NUM_CHANNELS> channels;

static void throw_error(std::string msg) {
	msg += SDL_GetError();
	throw(audio_exception(msg));
}

static void callback(void* userdata, uint8_t* stream, int len) {
	int16_t* str = (int16_t*)stream;
	for (int n = 0; n < len / 2; n++) {
		// bad mixing...
		*str++ = (getNextSample(channels[0]) / 4) + (getNextSample(channels[1]) / 4) +
		         (getNextSample(channels[2]) / 4) + (getNextSample(channels[3]) / 4);
	}
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
	spec.samples = 2048;
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

	if (SDL_LoadWAV(name, &wav.spec, (uint8_t**)&wav.sampleData, &wav.numSamples) == nullptr) {
		throw_error("SDL_LoadWav error: ");
	}

	wav.numSamples /= 2;

	SDL_LockAudioDevice(audioDevice);
	loadedWavs.push_back(wav);
	SDL_UnlockAudioDevice(audioDevice);

	return loadedWavs.size() - 1;
}

void AUDIO_Play(int id, int chan, bool loop) {
	Channel ci;
	ci.wav = &loadedWavs[id];
	ci.loop = loop;
	ci.playing = true;
	SDL_LockAudioDevice(audioDevice);
	channels[chan] = ci;
	SDL_UnlockAudioDevice(audioDevice);
}

bool AUDIO_Playing(int chan) {
	SDL_LockAudioDevice(audioDevice);
	bool playing = channels[chan].playing;
	SDL_UnlockAudioDevice(audioDevice);
	return playing;
}
