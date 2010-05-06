// vim: set noexpandtab:

#include <alure.h>
#include "audio.h"
#include "debug.h"
#include "game.h"
#include "sdl_exception.h"
#include "resource_manager.h"


namespace grail {

const Audio::Volume Audio::defaultVolume = 200;

Audio::SoundTask::SoundTask(size_t loops)
	: Task((loops == 0 ? Task::ENDLESS : Task::DEFAULT)) {
}

Audio::Audio() {
}

Audio::~Audio() {
}

Task::Ptr Audio::playSound(std::string resource, size_t loops, Audio::Volume v)
{
	ALuint src,buf;
	alureInitDevice(NULL,NULL);
	alGenSources(1, &src);
	buf = alureCreateBufferFromFile(resource.c_str());
	alSourcei(src, AL_BUFFER, buf);
}


} // namespace grail
