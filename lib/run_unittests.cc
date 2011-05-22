// vim: set noexpandtab:

#include <utility>
#include <boost/shared_ptr.hpp>
#include <SDL.h>

#include "unittest.h"

#include "vector2d.h"
#include "utils.h"
#include "task.h"
#include "wait_task.h"

using std::make_pair;

namespace grail {

TEST(Vector2d, nearestDirection) {
	CHECK_EQUAL((int)VirtualPosition(  0, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition(  3, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition(  6, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition(  9, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition( -3, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition( -6, 10).nearestDirection(4), 0);
	CHECK_EQUAL((int)VirtualPosition( -9, 10).nearestDirection(4), 0);
	
	CHECK_EQUAL((int)VirtualPosition( 10,  0).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10,  3).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10,  6).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10,  9).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10, -3).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10, -6).nearestDirection(4), 1);
	CHECK_EQUAL((int)VirtualPosition( 10, -9).nearestDirection(4), 1);
	
	CHECK_EQUAL((int)VirtualPosition(  0,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition(  3,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition(  6,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition(  9,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition( -3,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition( -6,-10).nearestDirection(4), 2);
	CHECK_EQUAL((int)VirtualPosition( -9,-10).nearestDirection(4), 2);
	
	CHECK_EQUAL((int)VirtualPosition(-10,  0).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10,  3).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10,  6).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10,  9).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10, -3).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10, -6).nearestDirection(4), 3);
	CHECK_EQUAL((int)VirtualPosition(-10, -9).nearestDirection(4), 3);
}

TEST(Utils, strip) {
	CHECK_EQUAL(lstrip(""), "");
	CHECK_EQUAL(lstrip("foobar"), "foobar");
	CHECK_EQUAL(lstrip("foo  bar   "), "foo  bar   ");
	CHECK_EQUAL(lstrip("  f o o b a r  "), "f o o b a r  ");
	CHECK_EQUAL(lstrip("     x"), "x");
	CHECK_EQUAL(lstrip("      "), "");
	
	CHECK_EQUAL(rstrip(""), "");
	CHECK_EQUAL(rstrip("foobar"), "foobar");
	CHECK_EQUAL(rstrip("   foo   bar"), "   foo   bar");
	CHECK_EQUAL(rstrip("  f o o b a r  "), "  f o o b a r");
	CHECK_EQUAL(rstrip("x    "), "x");
	CHECK_EQUAL(rstrip("      "), "");
	
	CHECK_EQUAL(strip(""), "");
	CHECK_EQUAL(strip("   foo   bar   "), "foo   bar");
	CHECK_EQUAL(strip("      "), "");
}

TEST(Utils, split) {
	CHECK_EQUAL(split2("foo = bar", "=").first, "foo ");
	CHECK_EQUAL(split2("foo = bar", "=").second, " bar");
	CHECK_EQUAL(split2("foo bar", "=").first, "foo bar");
	CHECK_EQUAL(split2("foo bar", "=").second, "");
	CHECK_EQUAL(split2("foo = bar = baz", "=").first, "foo ");
	CHECK_EQUAL(split2("foo = bar = baz", "=").second, " bar = baz");

	std::string s = "  foo bar baz  bang buff";
	SplitIterator iter = SplitIterator(s, " ");
	CHECK_EQUAL(*iter, ""); iter++;
	CHECK_EQUAL(*iter, ""); iter++;
	CHECK_EQUAL(*iter, "foo"); iter++;
	CHECK_EQUAL(*iter, "bar"); iter++;
	CHECK_EQUAL(*iter, "baz"); iter++;
	CHECK_EQUAL(*iter, ""); iter++;
	CHECK_EQUAL(*iter, "bang"); iter++;
	CHECK_EQUAL(*iter, "buff"); iter++;
	CHECK_EQUAL(iter, iter.end());
}

TEST(Utils, normalizePath) {
	CHECK_EQUAL(normalizePath("/"), "/");
	CHECK_EQUAL(normalizePath("//"), "/");
	CHECK_EQUAL(normalizePath("///"), "/");
	CHECK_EQUAL(normalizePath("foo"), "/foo");
	CHECK_EQUAL(normalizePath("/foo"), "/foo");
	CHECK_EQUAL(normalizePath("foo/"), "/foo");
	CHECK_EQUAL(normalizePath("/foo/"), "/foo");
	CHECK_EQUAL(normalizePath("//foo/"), "/foo");
	CHECK_EQUAL(normalizePath(""), "/");
	CHECK_EQUAL(normalizePath("////foo////bar////"), "/foo/bar");
}

TEST(Utils, math) {
	CHECK_EQUAL(nextPower2(0), 0);
	CHECK_EQUAL(nextPower2(1), 1);
	CHECK_EQUAL(nextPower2(2), 2);
	CHECK_EQUAL(nextPower2(3), 4);
	CHECK_EQUAL(nextPower2(4), 4);
	CHECK_EQUAL(nextPower2(5), 8);
	CHECK_EQUAL(nextPower2(6), 8);
	CHECK_EQUAL(nextPower2(7), 8);
	CHECK_EQUAL(nextPower2(8), 8);
	CHECK_EQUAL(nextPower2(800), 1024);
}


class DummyTask : public Task {
	public:
		typedef boost::shared_ptr<DummyTask> Ptr;
		bool onStart_executed;

		DummyTask() : Task(), onStart_executed(false) {
		}
		void onStart() { onStart_executed = true; }
		void end() { signalComplete(); }
};

TEST(Task, States) {
	DummyTask::Ptr t = DummyTask::Ptr(new DummyTask);
	CHECK_EQUAL(t->getState(), Task::STATE_NEW);
	t->start();
	CHECK_EQUAL(t->onStart_executed, true);
	CHECK_EQUAL(t->getState(), Task::STATE_RUNNING);
	t->end();
	CHECK_EQUAL(t->getState(), Task::STATE_COMPLETED);
}

TEST(Task, WaitTask) {
	// Blocking
	WaitTask::Ptr t = WaitTask::Ptr(new WaitTask(200));
	uint32_t now = SDL_GetTicks();
	CHECK_EQUAL(t->getState(), Task::STATE_NEW);
	t->block();
	CHECK_EQUAL(t->getState(), Task::STATE_COMPLETED);
	CHECK_GREATER(SDL_GetTicks(), now + 200);

	// Non-Blocking
	t = WaitTask::Ptr(new WaitTask(200));
	CHECK_EQUAL(t->getState(), Task::STATE_NEW);
	now = SDL_GetTicks();
	t->start();
	CHECK_EQUAL(t->getState(), Task::STATE_RUNNING);
	CHECK_LOWER(SDL_GetTicks(), now + 200);
}

} // namespace grail

int main(int argc, char** argv) {
	grail::Unittest::runAll();
	return 0;
}


