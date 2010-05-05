
#include "task.h"

namespace grail {

	Task::Task(Task::Flags flags) : flags(flags) {
	}

	Task::~Task() {
	}

	void Task::signalComplete() {
	}

	void Task::start() {
		onStart();
	}

	void Task::block() {
		start();
		// TODO: Do the actual mainloop magic here
	}

}


