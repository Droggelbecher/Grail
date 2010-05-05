// vim: set noexpandtab ts=4 sw=4:

#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "classes.h"

namespace grail {

	/**
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

		protected:
			/**
			 * Override this and it will be called when the task is being
			 * started.
			 */
			virtual void onStart() { };

			/**
			 * Override this and it shall be called once each frame, you may
			 * execute your task piecewise here.
			 */
			virtual void eachFrame(uint32_t ticks) { }

			Task(Flags flags = DEFAULT);

		
		public:
			virtual ~Task();

			Flags getFlags() { return flags; }

			/**
			 * Call this method when your task is complete, this will
			 * immediately exit the blocking mainloop if there is any.
			 *
			 * If you know you won't ever call this method, create the task
			 * object with the ENDLESS flag so it can be handled apropriately.
			 */
			void signalComplete();

			/**
			 * Run the task in a non-blocking fashion.
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

			friend class MainLoop;
	};

}

#endif // TASK_H

