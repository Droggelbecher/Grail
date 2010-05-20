// vim: set noexpandtab:

#include <alure.h>
#include "audio.h"
#include "debug.h"
#include "game.h"
#include "resource_manager.h"
#include "utils.h"
#include "sound_task.h"


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

Task::Ptr Audio::prepareSound(std::string resource, size_t loops, Audio::Volume v) //TODO: implement volume stuff
{
	SoundTask::Ptr ptr =  SoundTask::Ptr(new SoundTask(resource,loops));
	return ptr;
}


//for later alure api
//void Audio::alure_callback()
//{
//}

} // namespace grail
