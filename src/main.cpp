#include "audio.h"

#include <iostream>

#include <SDL2/SDL.h>

int main(void) {
	try {
		AUDIO_Init();
		AUDIO_LoadWav("xwing_41.wav");
		SDL_Delay(5000);

	} catch (audio_exception& err) {
		std::cerr << err.what() << std::endl;
	}
	return 0;
}
