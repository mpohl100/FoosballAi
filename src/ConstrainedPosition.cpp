#include "ConstrainedPosition.h"

ConstrainedPosition::ConstrainedPosition(double min, double max)
	: pos_(min), min_(min), max_(max)
{}

double ConstrainedPosition::pos() const { return pos_; }
double ConstrainedPosition::min() const { return min_; }
double ConstrainedPosition::max() const { return max_; }


void ConstrainedPosition::move(double x)
{
	pos_ += x;
	if (pos_ < min_) pos_ = min_;
	if (pos_ > max_) pos_ = max_;
}
