// vim: set noexpandtab ts=4 sw=4:

#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "classes.h"

namespace grail {
	
	/**
	 * Task-System for environments that does not depend on multithreading.
	 * Tasks can be either executed blockingly one after another, or
	 * multiplexed together in a mainloop which will exit when all tasks are
	 * done.
	 * 
	 * Note: This class (and all its subclasses) *must* be held in a
	 * shared_ptr after they have been initialized (else you will get a crash
	 * at start/block)
	 */
	class Task : public boost::enable_shared_from_this<Task> {
		public:
			typedef boost::shared_ptr<Task> Ptr;
			enum Flags {
				DEFAULT = 0x0,
				ENDLESS = 0x1,
			};
			
			enum State {
				STATE_NEW, STATE_RUNNING, STATE_COMPLETED
			};
		
		private:
			Flags flags;
			State state;
			MainLoop *loop;
			
			void setContainingLoop(MainLoop* loop);
		
		public:
			Task(Flags flags = DEFAULT);
			virtual ~Task();
			
			/// Return task flags
			Flags getFlags() { return flags; }
			
			/**
			 * Call this method when your task is complete, this will
			 * immediately exit the blocking mainloop if there is any.
			 *
			 * If you know you won't ever call this method, create the task
			 * object with the ENDLESS flag so it can be handled apropriately.
			 * Of course block() will block forever for endless tasks.
			 */
			void signalComplete();
			
			/**
			 * Run the task in a non-blocking fashion.
			 * (i.e. add it to the current game main loop)
			 */
			void start();
			
			/**
			 * Run the task in a blocking fashion (won't return before the
			 * task is done).
			 */
			void block();
			
			/**
			 * Return the current state of this task.
			 */
			State getState() const;
			
			/**
			 * Override this and it will be called when the task is being
			 * started. Use this to execute some initial/setup work, but don't
			 * have this method execute the hard stuff as this would effectively
			 * block the whole engine.
			 * 
			 * You usually don't want to call this method directly (rather use
			 * start() or block()).
			 */
			virtual void onStart() { };
			
			/**
			 * Override this and it shall be called once each frame, you may
			 * execute your task piecewise here.
			 * 
			 * You usually don't want to call this method directly (rather use
			 * start() or block()).
			 */
			virtual void eachFrame(uint32_t ticks) { }
			
			
			friend class MainLoop;
	};
	
	std::ostream& operator<<(std::ostream& os, const Task& ui);
	
}

#endif // TASK_H

