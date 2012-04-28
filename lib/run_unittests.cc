// vim: set noexpandtab:

#include <utility>
#include <boost/shared_ptr.hpp>
#include <SDL.h>

#include "unittest.h"

#include "vector2d.h"
#include "utils.h"
#include "task.h"
#include "wait_task.h"
#include "ground.h"
#include "actor.h"
#include "polygon.h"
#include "debug.h"

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

TEST(Line, realIntersect) {
	double eps = 1.0; //0.001;
	Line li(VirtualPosition(0,-10), VirtualPosition(0,10));
	
	for(double r = eps; r < M_PI; r += eps) {
		Line li2(VirtualPosition(-10.0*sin(r), -10.0*cos(r)), VirtualPosition(10.0*sin(r), 10.0*cos(r)));
		CHECK_EQUAL(li.intersects(li2, Line::TOUCH_OR_INTERSECT), true);
		CHECK_EQUAL(li2.intersects(li, Line::TOUCH_OR_INTERSECT), true);
		CHECK_EQUAL(li.intersects(li2, Line::REAL_INTERSECT), true);
		CHECK_EQUAL(li2.intersects(li, Line::REAL_INTERSECT), true);
	}
}

TEST(Line, touchIntersect) {
	double eps = 1.0; //0.001;
	Line li(VirtualPosition(0,-10), VirtualPosition(0,10));
	
	// "Weak" intersections (intersection point is one lines end point)
	// should be reportet as not intersecting and @ref reportWeak is false
	
	for(double r = eps; r < M_PI; r += eps) {
		Line li2(VirtualPosition(0,0), VirtualPosition(10.0*sin(r), 10.0*cos(r)));
		CHECK_EQUAL(li.intersects(li2, Line::TOUCH_OR_INTERSECT), true);
		CHECK_EQUAL(li2.intersects(li, Line::TOUCH_OR_INTERSECT), true);
		CHECK_EQUAL(li.intersects(li2, Line::REAL_INTERSECT), false);
		CHECK_EQUAL(li2.intersects(li, Line::REAL_INTERSECT), false);
	}
}

TEST(Polygon, LineIterator) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	polygon_t p;
	
	p.push_back(VirtualPosition(-5, -5));
	p.push_back(VirtualPosition(-5,  5));
	p.push_back(VirtualPosition( 5,  5));
	p.push_back(VirtualPosition( 5, -5));
	
	polygon_t::LineIterator li = p.beginLines();
	
	CHECK_EQUAL(*li, Line(VirtualPosition(-5, -5), VirtualPosition(-5, 5)));
	++li;
	CHECK_EQUAL(*li, Line(VirtualPosition(-5,  5), VirtualPosition( 5, 5)));
	++li;
	CHECK_EQUAL(*li, Line(VirtualPosition( 5,  5), VirtualPosition( 5,-5)));
	++li;
	CHECK_EQUAL(*li, Line(VirtualPosition( 5, -5), VirtualPosition(-5,-5)));
	//++li;
	//CHECK_EQUAL(li, p.endLines());
	
}

TEST(Polygon, orientation) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	polygon_t p;
	
	// CW triangle
	p.push_back(VirtualPosition(0, -10));
	p.push_back(VirtualPosition(10, 0));
	p.push_back(VirtualPosition(-10, 0));
	
	CHECK_EQUAL(p.getOrientation(), polygon_t::CW);
	
	// extend to strango CCW figure
	p.push_back(VirtualPosition(0, 10));
	p.push_back(VirtualPosition(20, 0));
	CHECK_EQUAL(p.getOrientation(), polygon_t::CCW);
	
	p.clear();
	p.push_back(VirtualPosition(0, -10));
	p.push_back(VirtualPosition(-10, 0));
	p.push_back(VirtualPosition(10, 0));
	CHECK_EQUAL(p.getOrientation(), polygon_t::CCW);
	
	p.clear();
	p.push_back(VirtualPosition(-5, -5));
	p.push_back(VirtualPosition( 5, -5));
	p.push_back(VirtualPosition( 5,  5));
	p.push_back(VirtualPosition(-5,  5));
	CHECK_EQUAL(p.getOrientation(), polygon_t::CW);
	
	p.clear();
	p.push_back(VirtualPosition(-5, -5));
	p.push_back(VirtualPosition(-5,  5));
	p.push_back(VirtualPosition( 5,  5));
	p.push_back(VirtualPosition( 5, -5));
	CHECK_EQUAL(p.getOrientation(), polygon_t::CCW);
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

TEST(Ground, directReachable) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	/*
		p11------------------------------p14    - 0
		|                                 |
		|    p21---p22       p33---p32    |     - 20
		|    |      |   x1   |      |     |
		|    |      |        |  x2  |     |     - 40
		| x3 |  x4  |        |      |     |     - 50
		|    |      |        |  x5  |     |     - 60
		|    |      |        |      |     |
		|    p24---p23       p34---p31    |     - 80
		|                                 |
		|              x6                 |     - 90
		|                                 |
		p12------------------------------p13    - 100
		
		|    |  |   |  | |   |  |   |     |
		0    20 50  80 90100 120150 180   200
		
	*/

	
	// p11 p12 p13 p14   p21 p22 p23 p24   p31 p32 p33 p34  x1 x2 x3 x4 x5 x6
	enum Idx { P11=0, P12, P13, P14, P21, P22, P23, P24, P31, P32, P33, P34, X1, X2, X3, X4, X5, X6 };
	typedef VirtualPosition P;
	P pos[] = {
		P(0, 0), P(0, 100), P(200, 100), P(200, 0),
		P(20, 20), P(80, 20), P(80, 80), P(20, 80),
		P(180, 80), P(180, 20), P(120, 20), P(120, 80),
		P(100, 30), P(150, 40), P(10, 50), P(50, 50), P(150, 60), P(90, 90)
	};
	
	Ground g;
	
	polygon_t p1, p2, p3;
	
	p1.push_back(pos[P11]);
	p1.push_back(pos[P12]);
	p1.push_back(pos[P13]);
	p1.push_back(pos[P14]);
	g.addPolygon(p1);
	
	p2.push_back(pos[P21]);
	p2.push_back(pos[P22]);
	p2.push_back(pos[P23]);
	p2.push_back(pos[P24]);
	g.addPolygon(p2);
	
	p3.push_back(pos[P31]);
	p3.push_back(pos[P32]);
	p3.push_back(pos[P33]);
	p3.push_back(pos[P34]);
	g.addPolygon(p3);
	
	// Root component
	
	char reachable[X6+1][X6+2] = {
		"110111010110001000", // p11
		"111010111001001001", // p12
		"011100111101000001", // p13
		"101111001110000000", // p14
		"110111010000001000", // p21
		"100111100011100001", // p22
		"011001110011100001", // p23
		"111010110000001001", // p24
		"011100001101000001", // p31
		"101100001110000000", // p32
		"100101100111100001", // p33
		"011001101011100001", // p34
		"000001100011100001", // x1
		"000000000000000000", // x2
		"110010010000000000", // x3
		"000000000000000000", // x4
		"000000000000000000", // x5
		"011001111011100001"  // x6
	};
	
	// check symmetry of reachable[]
	for(int i = 0; i<=X6; ++i) {
		for(int j = 0; j<=X6; ++j) {
			assert(reachable[i][j] == reachable[j][i]);
		}
	}
	
	g.rootComponent->generateWaypoints();
	
	for(int i = 0; i<=X6; ++i) {
		for(int j = 0; j<=X6; ++j) {
			//bool expected = (reachable[i][j] == '1');
			if(i <= P34 && j <= P34) {
				//cdbg << "i=" << i << " j=" << j << "\n";
				//cdbg
				//	<< "wp[i]=" << g.rootComponent->waypoints[i].getPosition()
				//	<< " wp[j]=" << g.rootComponent->waypoints[j].getPosition()
				//	<< "\n";
				if(reachable[i][j] == '1') {
					info("i=%d j=%d 1", i, j);
					CHECK_EQUAL(
						g.directReachable(
							g.rootComponent,
							g.rootComponent->waypoints[i],
							g.rootComponent->waypoints[j]
						), 1
					);
				}
				else {
					info("i=%d j=%d 0", i, j);
					CHECK_EQUAL(
						g.directReachable(
							g.rootComponent,
							g.rootComponent->waypoints[i],
							g.rootComponent->waypoints[j]
						), 0
					);
				}
			
			} // if
		} // for j
	} // for i
}

/*
TEST(Ground, Pathfinding) {
	Ground g;
	
	Polygon<VirtualPosition, IsPosition> poly;
	
	poly.push_back(VirtualPosition(100, 100));
	poly.push_back(VirtualPosition(100, 200));
	poly.push_back(VirtualPosition(200, 200));
	poly.push_back(VirtualPosition(200, 100));
	g.addWalls(poly);
	
	CHECK_EQUAL(g.directReachable(VirtualPosition(50, 150), VirtualPosition(250, 150)), false);
	CHECK_EQUAL(g.directReachable(VirtualPosition(40, 150), VirtualPosition(250, 140)), false);
	
	// find a path around the square obstacle
	Path p;
	g.getPath(VirtualPosition(50, 140), VirtualPosition(250, 140), p);
	CHECK_EQUAL(p.size(), 3);
	CHECK_EQUAL(p.back(), VirtualPosition(250, 140));
	
	// now find a path to a wall-intersection point
	// (that should be disallowed and thus return an empty path)
	p.clear();
	g.getPath(VirtualPosition(50, 140), VirtualPosition(100, 100), p);
	CHECK_EQUAL(p.size(), 0);
	
	// However it is allowed to have a waypoint as starting point
	p.clear();
	g.getPath(VirtualPosition(100, 100), VirtualPosition(150, 150), p);
	CHECK_EQUAL(p.size(), 1);
	
	p.clear();
	g.getPath(VirtualPosition(50, 150), VirtualPosition(150, 150), p);
	CHECK_EQUAL(p.size(), 0);
	
	p.clear();
	g.getPath(VirtualPosition(50, 50), VirtualPosition(150, 150), p);
	CHECK_EQUAL(p.size(), 0);
	
	p.clear();
	g.getPath(VirtualPosition(100, 150), VirtualPosition(101, 150), p);
	CHECK_EQUAL(p.size(), 0);
}
*/


} // namespace grail

int main(int argc, char** argv) {
	grail::Unittest::runAll();
	return 0;
}


