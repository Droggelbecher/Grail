// vim: set noexpandtab:

#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL.h>

namespace grail {

class MainLoop {
	bool exit_;
	double targetFPS;
	
	public:
		MainLoop() : exit_(false), targetFPS(50.0) { };
		virtual ~MainLoop() { };
		
		virtual void run();
		virtual void exit();
		
		virtual void handleEvent(SDL_Event& event, uint32_t frameDuration);
};

} // namespace grail

#endif // MAINLOOP_H

