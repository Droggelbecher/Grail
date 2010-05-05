// vim: set noexpandtab:

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


} // namespace grail
