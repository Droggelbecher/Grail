// vim: set noexpandtab:

#ifndef SOUND_TASK_H
#define SOUND_TASK_H

#include <string>
#include <stdint.h>
#include <AL/alure.h> //hm, why doesnt <alure.h> work?
#include "task.h"

namespace grail {

class SoundTask : public Task {
		
		//TODO: which variables are really needed? and look further at openal
		size_t loops;

		ALuint src,alBuf;
		size_t bufsize;
		const ALubyte* buffer;

	public:
		SoundTask(std::string resource, size_t loops);
		//TODO: free openal resources in destructor
		void onStart();

		// Sound specific
		void pause();
		void setVolume();
		typedef uint8_t Volume;
		static const Volume defaultVolume;

		void eachFrame(uint32_t ticks);
};

}

#endif //SOUND_TASK_H
