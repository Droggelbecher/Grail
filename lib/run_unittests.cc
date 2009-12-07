
#include <utility>

#include "unittest.h"

#include "vector2d.h"
#include "utils.h"

using std::make_pair;

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

TEST(utils, strip) {
  CHECK_EQUAL(lstrip(""), "");
  CHECK_EQUAL(lstrip("foobar"), "foobar");
  CHECK_EQUAL(lstrip("foo   bar   "), "foo   bar   ");
  CHECK_EQUAL(lstrip("  f o o b a r  "), "f o o b a r  ");
  CHECK_EQUAL(lstrip("     x"), "x");
  CHECK_EQUAL(lstrip("      "), "");

  CHECK_EQUAL(rstrip(""), "");
  CHECK_EQUAL(rstrip("foobar"), "foobar");
  CHECK_EQUAL(rstrip("   foo   bar"), "   foo   bar");
  CHECK_EQUAL(rstrip("  f o o b a r  "), "  f o o b a r");
  CHECK_EQUAL(rstrip("x     "), "x");
  CHECK_EQUAL(rstrip("      "), "");

  CHECK_EQUAL(strip(""), "");
  CHECK_EQUAL(strip("   foo   bar   "), "foo   bar");
  CHECK_EQUAL(strip("       "), "");
}

TEST(utils, split) {
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

int main(int argc, char** argv) {
  Unittest::runAll();
  return 0;
}

