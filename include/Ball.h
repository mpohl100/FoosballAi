#include "ConstrainedPosition.h"
#include "FoosballMeasures.h"
#pragma once

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
