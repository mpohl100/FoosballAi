#include "ConstrainedPosition.h"

#pragma once

const double FIGURE_LEN = 73.75;
constexpr double CENTER_ZONES = 5;
constexpr double RELATIVE_ZONES = 5;
const double GOAL_DIST = 205;
const double FIELD_MIN_X = 0;
const double FIELD_MIN_Y = -235;
const double FIELD_MAX_X = 1200;
const double FIELD_MAX_Y = 440;
const double FIELD_FIGURE_OFFSET = 15;
const double ROD_DIST = 147.5;
const double FIGURE_FOOT = 22;

class Ball {
public:
	Ball() = default;
	Ball(Ball const&) = default;
	Ball& operator=(Ball const&) = default;
	Ball(Ball&&) = default;
	Ball& operator=(Ball&&) = default;

	Ball(double x, double y);
	double x() const;
	double y() const;

	static double perimeter(double rod_y);

private:
	ConstrainedPosition x_{ FIELD_MIN_X, FIELD_MAX_X };
	ConstrainedPosition y_{ FIELD_MIN_Y, FIELD_MAX_Y };
};

bool operator<(Ball const& l, Ball const& r);
