// vim: set noexpandtab ts=4 sw=4:

#ifndef TASK_H
#define TASK_H

#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace grail {

	class Task {
		public:
			typedef boost::shared_ptr<Task> Ptr;
			enum Flags {
				DEFAULT = 0x0,
				ENDLESS = 0x1,
			};
			
		private:
			Flags flags;

		protected:
			virtual void eachFrame(uint32_t ticks) { }
			virtual void onStart() { };
		
		public:
			Task(Flags flags);
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

			void start();
			void block();
	};
}

#endif // TASK_H

