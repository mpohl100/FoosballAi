#include "Point.h"

Point Point::operator+(double d)
{
	return Point( x + d, y + d );
}

Point Point::operator-(double d)
{
	return Point( x - d, y - d );
}

bool operator==(Point const& l, Point const& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator!=(Point const& l, Point const& r)
{
	return !(l == r);
}

bool operator<(Point const& l, Point const& r)
{
	if (l.x != r.x)
		return l.x < r.x;
	return l.y < r.y;
}

bool operator<=(Point const& l, Point const& r)
{
	return !(l > r);
}

bool operator>(Point const& l, Point const& r)
{
	return r < l;
}

bool operator>=(Point const& l, Point const& r)
{
	return !(l < r);
}