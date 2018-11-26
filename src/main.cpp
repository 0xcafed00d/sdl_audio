#include "audio.h"

#include <iostream>

#include <SDL2/SDL.h>

int main(void) {
	try {
		AUDIO_Init();
		int musicid = AUDIO_LoadWav("xwing_41.wav");
		int sfx0id = AUDIO_LoadWav("jetdude0.wav");
		int sfx1id = AUDIO_LoadWav("jetdude1.wav");
		int sfx2id = AUDIO_LoadWav("jetdude2.wav");
		int sfx3id = AUDIO_LoadWav("jetdude3.wav");
		int sfx4id = AUDIO_LoadWav("jetdude4.wav");

		AUDIO_Play(musicid, 0, true);

		while (AUDIO_isPlaying(0)) {
			if (!AUDIO_isPlaying(1))
				AUDIO_Play(sfx0id, 1, false);
		}
	} catch (audio_exception& err) {
		std::cerr << err.what() << std::endl;
	}
	return 0;
}
