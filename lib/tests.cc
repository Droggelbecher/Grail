
#include "unittest.h"
#include "vector2d.h"

TEST(Vector2d, nearestDirection) {

  CHECK_EQUAL( VirtualPosition(-10, -10).nearestDirection(4), 0 );
  CHECK_EQUAL( VirtualPosition(-10,  10).nearestDirection(4), 1 );
  CHECK_EQUAL( VirtualPosition( 10,  10).nearestDirection(4), 2 );
  CHECK_EQUAL( VirtualPosition( 10, -10).nearestDirection(4), 3 );

}


