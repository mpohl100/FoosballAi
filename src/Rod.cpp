#include "Rod.h"
#include "Figure.h"

Rod::Rod(std::vector<Figure> figures, double x)
	: figures_(figures), x_(x)
{}

bool Rod::touchesShot(Shot const& shot) const
{
	for (const auto& f : figures_)
		if (f.touchesShot(shot, x_))
			return true;
	return false;
}

void Rod::move_y(double y)
{
	for (auto& f : figures_)
		f.move_y(y);
}

void Rod::move_y_to(double y)
{
	double current_y = figures_[0].y();
	for (auto& f : figures_)
		f.move_y(y-current_y);
}

void Rod::move_rod(double x)
{
	for (auto& f : figures_)
		f.move_x(x);
}

double Rod::y() const
{
	return figures_[0].y();
}

double Rod::x() const
{
	return x_;
}


std::pair<double, double> Rod::range_y() const
{
	return figures_[0].range_y();
}

std::vector<Figure> const& Rod::getFigures() const
{
	return figures_;
}

std::ostream& operator<<(std::ostream& os, Rod const& rod)
{
	int BASE_LEN = 10;
	int nbCols = (FIELD_MAX_Y - FIELD_MIN_Y) / BASE_LEN + 2;
	std::vector<double> pos;
	for (const auto& figure : rod.figures_)
		pos.push_back(figure.y());
	for (int i = -24; i < nbCols-24; ++i) {
		if (!pos.empty() && double(i)*BASE_LEN <= pos[0] && pos[0] <= double(i + 1) * BASE_LEN) {
			os << "MM";
			i++;
			pos.erase(pos.begin());
		}
		else
			os << " ";
	}
	return os;
}

