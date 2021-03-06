// vim: set noexpandtab ts=4 sw=4:


#ifndef WAIT_TASK_H
#define WAIT_TASK_H

#include <stdint.h>
#include "task.h"

namespace grail {
	
	/**
	 * Task that does nothing for a given duration and then completes.
	 */
	class WaitTask : public Task {
		private:
			uint32_t duration;
			uint32_t expire;
		
		public:
			/**
			 * \param duration Sleep duration in ms.
			 */
			WaitTask(uint32_t duration);
			~WaitTask() { };
			
			void onStart();
			void eachFrame(uint32_t ticks);
	};
	
}

#endif // WAIT_TASK_H

