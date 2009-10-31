
#include "unittest.h"

#include "vector2d.h"

TEST(Vector2d, nearestDirection) {
  CHECK_EQUAL( (int)VirtualPosition(-10, -10).nearestDirection(4), 0 );
  CHECK_EQUAL( (int)VirtualPosition(-10,  10).nearestDirection(4), 1 );
  CHECK_EQUAL( (int)VirtualPosition( 10,  10).nearestDirection(4), 2 );
  CHECK_EQUAL( (int)VirtualPosition( 10, -10).nearestDirection(4), 3 );

  CHECK_EQUAL( (int)VirtualPosition(-10,   0).nearestDirection(4), 0 );
  CHECK_EQUAL( (int)VirtualPosition(  0,  10).nearestDirection(4), 1 );
  CHECK_EQUAL( (int)VirtualPosition( 10,   0).nearestDirection(4), 2 );
  CHECK_EQUAL( (int)VirtualPosition(  0, -10).nearestDirection(4), 3 );
}

int main(int argc, char** argv) {
  Unittest::runAll();
  return 0;
}

