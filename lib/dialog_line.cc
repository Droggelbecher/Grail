// vim: set noexpandtab:

#include "dialog_line.h"

namespace grail {

	DialogLine::DialogLine(Actor::Ptr spk, std::string s, uint32_t t) : speaker(spk), text(s) {
		timer = WaitTask::Ptr(new WaitTask(t));
	}

	void DialogLine::start() {
		if (!isStarted()) {
			if (timer->getState() == Task::STATE_NEW) {
				timer->block();
			}
		}
	}

	bool DialogLine::isStarted() {
		if (timer->getState() == Task::STATE_NEW || timer->getState() == Task::STATE_COMPLETED) {
			return false;
		}
		return true;
	}

	bool DialogLine::isComplete() {
		if (timer->getState() == Task::STATE_COMPLETED) {
			return true;
		}
		return false;
	}

	void DialogLine::eachFrame() {
	}

} //namespace grail

