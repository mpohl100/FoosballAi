#pragma once

#include "Point.h"
#include "ConstrainedPosition.h"
#include "Ball.h"
#include "FoosballMeasures.h"
#include "Shot.h"
#include <utility>

class Figure {
public:
	Figure(ConstrainedPosition y, ConstrainedPosition x = { -FIGURE_LEN,FIGURE_LEN });

	Figure() = default;
	Figure(Figure const&) = default;
	Figure& operator=(Figure const&) = default;
	Figure(Figure&&) = default;
	Figure& operator=(Figure&&) = default;

	int touchesShot(Shot shot, double x) const;

	void move_y(double y);
	void move_x(double x);

	double y() const;
	double x() const;

	std::pair<double, double> range_y() const;

private:
	double length_ = FIGURE_FOOT;
	double width_ = 8.0;
	ConstrainedPosition y_{ 0, GOAL_DIST };
	ConstrainedPosition x_{ -FIGURE_LEN, FIGURE_LEN };
};
