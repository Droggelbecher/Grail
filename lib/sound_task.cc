// vim: set noexpandtab:

#include "debug.h"
#include "game.h"
#include "resource_manager.h"
#include "utils.h"
#include "sound_task.h"

namespace grail {

SoundTask::SoundTask(std::string resource, size_t loops)
	: Task((loops == 0 ? Task::ENDLESS : Task::DEFAULT)) {

	alGenSources(1, &src);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to create OpenAL source! " << alGetError() << std::endl;

	Resource soundChunk(resource,MODE_READ);

	buffer = static_cast<const ALubyte*>(soundChunk.createBuffer(bufsize));

	alBuf = alureCreateBufferFromMemory(buffer,bufsize);
	// assert(alBuf);

	alSourcei(src, AL_BUFFER, alBuf);
	// TODO: make this more elegant and think about the api
	// TODO: Can "buffer" now be deleted? Or when song is played? Anyway, free it
	// at some point!
	// TODO: Same thought for alBuf
}

void SoundTask::eachFrame(uint32_t ticks) {
	alureUpdate();
}

static void eos_callback(void *unused, ALuint unused2)
{
		cdbg << "******* callback\n\n";
}

void SoundTask::onStart()
{
	alurePlaySource(src, &eos_callback, 0);

	if(alGetError() != AL_NO_ERROR)
		std::cout << "Failed to start source! " << alGetError() << std::endl;
}

void SoundTask::pause()
{
	alurePauseSource(src);
}

void SoundTask::setVolume()
{
	
}

}
