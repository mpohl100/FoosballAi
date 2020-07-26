#pragma once

#include "Figure.h"
#include <vector>
#include <iostream>

class Team;

class Rod {
public:
	Rod(std::vector<Figure> figures, double x);

	Rod() = default;
	Rod(Rod const&) = default;
	Rod& operator=(Rod const&) = default;
	Rod(Rod&&) = default;
	Rod& operator=(Rod&&) = default;

	bool touchesShot(Shot const& shot) const;

	void move_y(double y);
	void move_y_to(double y);
	void move_rod(double x);
	double y() const;
	double x() const;
	std::pair<double, double> range_y() const;
	std::vector<Figure> const& getFigures() const;
private:
	friend std::ostream& operator<<(std::ostream& os, Rod const& rod);
	friend class Team;
	std::vector<Figure> figures_;
	double x_;
};

std::ostream& operator<<(std::ostream& os, Rod const& rod);