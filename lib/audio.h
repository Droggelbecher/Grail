// vim: set noexpandtab:

#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <stdint.h>
#include "sound_task.h"

namespace grail {

class Audio {
	public:

		enum PlaylistFlags {
			PLAYLIST_DEFAULT = 0x00,
			PLAYLIST_LOOP = 0x01,        // after last track, start with first one again
			PLAYLIST_LOOP_LAST = 0x02,   // loop last track
			PLAYLIST_CROSSFADING = 0x04  // enable crossfading between tracks
		};

		Audio();
		~Audio();

		/**
		 * Audio a;
		 *
		 * // Blocks execution flow until sound is played
		 * a.playSound("foo.ogg").block();
		 *
		 * // Plays sound in background
		 * a.playSound("foo.ogg").start();
		 *
		 * // Runs playSound and walkTo in parallel, blocking until
		 * // both tasks are done.
		 * MainLoop l;
		 * l.addTask(a.playSound("foo.ogg"));
		 * l.addTask(foo.walkTo(VirtualPosition(x, y)));
		 * l.run()
		 *
		 * loops=0 --> endless repeat
		 *
		 */
		SoundTask::Ptr prepareSound(std::string resource, size_t loops = 1);

		/* Playlist stuff
		void clearPlaylist();
		void appendToPlaylist(std::string resource, PlaylistFlags flags = PLAYLIST_DEFAULT);
		Volume getPlaylistVolume() const;
		void setPlaylistVolume();
		
		PlaylistTask::Ptr fadeOutPlaylist(uint32_t ms);
		PlaylistTask::Ptr fadeInPlaylist(uint32_t ms);
		void startPlaylist();
		void stopPlaylist();
		void setPlaylistCrossfadingDuration(uint32_t ms);
		*/
		//static void alure_callback();
};



} // namespace grail

#endif // AUDIO_H
