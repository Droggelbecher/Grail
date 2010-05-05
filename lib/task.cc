
#include "task.h"

namespace grail {

	Task::Task(Task::Flags flags) : flags(flags), state(Task::STATE_NEW) {
	}

	Task::~Task() {
	}

	void Task::signalComplete() {
		state = STATE_COMPLETED;
	}

	void Task::start() {
		assert(state == Task::STATE_NEW);
		state = STATE_RUNNING;
		onStart();
	}

	void Task::block() {
		assert(state == Task::STATE_NEW);
		start();
		// TODO: Do the actual mainloop magic here
	}

	Task::State Task::getState() const {
		return state;
	}

}


