#pragma once

struct Point
{
	double x = 0.0;
	double y = 0.0;
	
	Point(double xx, double yy ) : x(xx) , y(yy) {}
	Point() = default;
	Point(Point const&) = default;
	Point& operator=(Point const&) = default;
	Point(Point&&) = default;
	Point& operator=(Point&&) = default;

	Point operator+(double d);
	Point operator-(double d);
};

bool operator==(Point const& l, Point const& r);
bool operator!=(Point const& l, Point const& r);

bool operator<(Point const& l, Point const& r);
bool operator<=(Point const& l, Point const& r);
bool operator>(Point const& l, Point const& r);
bool operator>=(Point const& l, Point const& r);
