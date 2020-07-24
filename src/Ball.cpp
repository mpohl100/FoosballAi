#include "Ball.h"

Ball::Ball(double x, double y)
	: x_(ConstrainedPosition( FIELD_MIN_X, FIELD_MAX_X ))
	, y_(ConstrainedPosition( FIELD_MIN_Y, FIELD_MAX_Y ))
{
	x_.move(x);
	y_.move(y-FIELD_MIN_Y);
}

double Ball::x() const { return x_.pos(); }
double Ball::y() const { return y_.pos(); }

double Ball::perimeter(double rod_y)
{
	return 12.5;
}

bool operator<(Ball const& l, Ball const& r)
{
	if (l.x() != r.x())
		return l.x() < r.x();
	return l.y() < r.y();
}
