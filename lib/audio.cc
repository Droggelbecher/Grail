// vim: set noexpandtab:

#include <alure.h>
#include "audio.h"
#include "debug.h"
#include "game.h"
#include "resource_manager.h"
#include "utils.h"


namespace grail {

const Audio::Volume Audio::defaultVolume = 200;

Audio::SoundTask::SoundTask(size_t loops)
	: Task((loops == 0 ? Task::ENDLESS : Task::DEFAULT)) {
}

Audio::Audio() {
	if (!alureInitDevice(NULL,NULL))
		throw Exception("OpenAL couldn't init the Sounddevice");
}

Audio::~Audio() {
}

Task::Ptr Audio::playSound(std::string resource, size_t loops, Audio::Volume v)
{
	ALuint src,alBuf;
	size_t bufsize;
	const ALubyte* buffer;

	alGenSources(1, &src);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to create OpenAL source! " << alGetError() << std::endl;

	Resource soundChunk(resource,MODE_READ);

	buffer = reinterpret_cast<const ALubyte*>(soundChunk.getData()); //fixme: static_cast doesnt work
	bufsize = soundChunk.getDataSize();

	alBuf = alureCreateBufferFromMemory(buffer,bufsize);

	alSourcei(src, AL_BUFFER, alBuf);
	alSourcePlay(src);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to start source! " << alGetError() << std::endl;

	return SoundTask::Ptr(new SoundTask(3));
}

//for later alure api
//void Audio::alure_callback()
//{
//}

} // namespace grail
