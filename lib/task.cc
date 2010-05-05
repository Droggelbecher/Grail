
#include "task.h"
#include "mainloop.h"
#include "debug.h"
#include "game.h"

namespace grail {

	Task::Task(Task::Flags flags) : flags(flags), state(Task::STATE_NEW), loop(0) {
	}

	Task::~Task() {
	}

	void Task::setContainingLoop(MainLoop* loop) {
		this->loop = loop;
	}

	void Task::signalComplete() {
		state = STATE_COMPLETED;
		if(loop) {
			loop->notifyCompleted(this);
		}
	}

	void Task::start() {
		assert(state == Task::STATE_NEW);
		state = STATE_RUNNING;
		onStart();
		Task::Ptr ptr(this);
		Game::getInstance().getMainLoop().addTask(ptr);
	}

	void Task::block() {
		assert(state == Task::STATE_NEW);
		state = STATE_RUNNING;
		onStart();
		MainLoop m;
		Task::Ptr ptr(this);
		m.addTask(ptr);
		m.run();
	}

	Task::State Task::getState() const {
		return state;
	}

}


