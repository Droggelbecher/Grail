// vim: set noexpandtab:

#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <SDL_mixer.h>

namespace grail {

class Audio {
	public:
		Audio();
		~Audio();
		void playSound(std::string soundfile);
	private:
		Mix_Music *sound;
};

} // namespace grail

#endif // AUDIO_H
