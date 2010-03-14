// vim: set noexpandtab:

#include "audio.h"
#include "debug.h"
#include "game.h"
#include "sdl_exception.h"
#include "resource_manager.h"

namespace grail {

Audio::Audio() {
	if(Mix_OpenAudio(44100, AUDIO_S16, 2, 4096)) {
		throw SDLException(std::string("Couldnt open Audio"));
	}
	
	sound = 0;
}

Audio::~Audio() {
	Mix_CloseAudio();
}

void Audio::playSound(std::string path) {
	SDL_RWops *rw = Game::getInstance().getResourceManager().getRW(path, MODE_READ);
	sound = Mix_LoadMUS_RW(rw);
	if(!sound) {
		throw SDLException(std::string("Couldnt open Soundfile: " + path));
	}
	
	Mix_PlayMusic(sound, -1);
	// TODO: quite everything: only loops indefinetly now, only music, i have to look more clearly at the sdl-mixer api, manage multiple sounds etc...
}

} // namespace grail
