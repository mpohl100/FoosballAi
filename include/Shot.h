#pragma once

#include "Point.h"
#include "Ball.h"

class Shot {
public:
	Shot(Ball start, Ball end);
	Point start_ = Point( 0,0 );
	Point end_ = Point( 0,0 );
};