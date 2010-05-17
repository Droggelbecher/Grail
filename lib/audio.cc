// vim: set noexpandtab:

#include <alure.h>
#include "audio.h"
#include "debug.h"
#include "game.h"
#include "resource_manager.h"
#include "utils.h"


namespace grail {

const Audio::Volume Audio::defaultVolume = 200;

Audio::Audio() {
	if (!alureInitDevice(NULL,NULL))
		throw Exception("OpenAL couldn't init the Sounddevice"); //TODO: think about exceptions and find out which openal stuff has to be caught
}

Audio::~Audio() {
	if (!alureShutdownDevice())
		throw Exception("Couldnt shutdown the Sounddevice");
}

SoundTask::SoundTask(std::string resource, size_t loops)
	: Task((loops == 0 ? Task::ENDLESS : Task::DEFAULT)) {

	alGenSources(1, &src);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to create OpenAL source! " << alGetError() << std::endl;

	Resource soundChunk(resource,MODE_READ);

	buffer = reinterpret_cast<const ALubyte*>(soundChunk.getData()); //fixme: static_cast doesnt work
	bufsize = soundChunk.getDataSize();

	alBuf = alureCreateBufferFromMemory(buffer,bufsize);

	alSourcei(src, AL_BUFFER, alBuf);
	//TODO: make this more elegant and think about the api
}

Task::Ptr Audio::prepareSound(std::string resource, size_t loops, Audio::Volume v) //TODO: implement volume stuff
{
	SoundTask::Ptr ptr =  SoundTask::Ptr(new SoundTask(resource,loops));
	return ptr;
}


void SoundTask::onStart()
{
	alSourcePlay(src);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to start source! " << alGetError() << std::endl;
}

void SoundTask::pause()
{
	alSourcePause(src);
}

//for later alure api
//void Audio::alure_callback()
//{
//}

} // namespace grail
