// vim: set noexpandtab:

#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL.h>
#include <list>
#include "classes.h"
#include "task.h"

namespace grail {

class MainLoop {
		bool exit_; // exit "signal"
		static double targetFPS;
		bool scheduleTaskCheck; // true when tasks have to be checked for completion
		bool master;
		std::list<Task::Ptr> tasks;
		
		void eachFrame(uint32_t frameDuration);
		void checkTasks();
		
	public:
		MainLoop(bool master=false) : exit_(false), master(master) { };
		virtual ~MainLoop() { };
		
		virtual void run();
		virtual void exit();
		
		virtual void handleEvent(SDL_Event& event, uint32_t frameDuration);

		/**
		 * Add task to this mainloop.
		 * A MainLoop with master=false will exit
		 * automatically as soon as all of its assigned tasks are completed.
		 */
		void addTask(Task::Ptr task);

		/**
		 * Notify loop that the given task has been completed.
		 * Will be called from the task itself so there should be no reason for
		 * you to call it manually.
		 */
		void notifyCompleted(Task* task);

		/**
		 * Set target FPS value for all main loops.
		 */
		static void setTargetFPS(double fps) { targetFPS = fps; }
};

} // namespace grail

#endif // MAINLOOP_H

