#pragma once

class ConstrainedPosition {
public:
	ConstrainedPosition(double min, double max);
	ConstrainedPosition() = default;
	ConstrainedPosition(ConstrainedPosition const&) = default;
	ConstrainedPosition& operator=(ConstrainedPosition const&) = default;
	ConstrainedPosition(ConstrainedPosition&&) = default;
	ConstrainedPosition& operator=(ConstrainedPosition&&) = default;

	double pos() const;
	double min() const;
	double max() const;
	void move(double x);
private:
	double pos_ = 0.0; // Position
	double min_ = 0.0;
	double max_ = 0.0;
};
