#include "Kicker.h"

#include <cmath>
#include <vector>
#include <string>
#include <array>
#include <numeric>
#include <algorithm>

int fetchRand(int range_from, int range_to)
{
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(range_from, range_to);
	return distr(rand_dev);
}


Goalie::Goalie(double y, double x)
	: rod({ Figure(ConstrainedPosition(0,GOAL_DIST - FIGURE_FOOT)) }, FIGURE_LEN)
{
	rod.move_y(y);
	rod.move_rod(x);
}

Defense::Defense(double y, double x)
	: rod({ Figure(ConstrainedPosition(-GOAL_DIST,195)),
			Figure(ConstrainedPosition(0,400)) }
		, FIGURE_LEN + ROD_DIST)
{
	rod.move_y(y);
	rod.move_rod(x);
}

double Team::touchesShot(Shot const& shot) const
{
	std::array<int, 4> rets;
	rets[0] = goalie_.rod.touchesShot(shot);
	rets[1] = defense_.rod.touchesShot(shot);
	rets[2] = midfield_.rod.touchesShot(shot);
	rets[3] = offense_.rod.touchesShot(shot);
	int sum = std::accumulate(rets.begin(), rets.end(), 0);
	if (sum > 0)
		return double(*std::max_element(rets.begin(), rets.end())) / double(sum);
	return 0.0;
}


std::pair<Point, Point> getShotsArea(std::vector<Shot> const& shots)
{
	Point lower;
	Point upper;
	std::vector<Point> allPoints;
	for (Shot const& shot : shots) {
		allPoints.push_back(shot.start_);
		allPoints.push_back(shot.end_);
	}
	auto x_min = [](Point l, Point r) { return l.x < r.x; };
	auto y_min = [](Point l, Point r) { return l.y < r.y; };
	auto it = std::min_element(allPoints.begin(), allPoints.end(), x_min);
	lower.x = it->x;
	it = std::min_element(allPoints.begin(), allPoints.end(), y_min);
	lower.y = it->y;
	it = std::max_element(allPoints.begin(), allPoints.end(), x_min);
	upper.x = it->x;
	it = std::max_element(allPoints.begin(), allPoints.end(), y_min);
	upper.y = it->y;
	return { lower, upper };
}

std::vector<Figure> Team::getRelevantFigures(std::pair<Point, Point> rect) const
{
	std::vector<Figure> figures;
	if(rect.first.x <= goalie_.rod.x() && goalie_.rod.x() <= rect.second.x)
		figures.insert(figures.end(), goalie_.rod.figures_.begin(), goalie_.rod.figures_.end());
	if (rect.first.x <= defense_.rod.x() && defense_.rod.x() <= rect.second.x)
		figures.insert(figures.end(), defense_.rod.figures_.begin(), defense_.rod.figures_.end());
	if (rect.first.x <= midfield_.rod.x() && midfield_.rod.x() <= rect.second.x)
		figures.insert(figures.end(), midfield_.rod.figures_.begin(), midfield_.rod.figures_.end());
	if (rect.first.x <= offense_.rod.x() && offense_.rod.x() <= rect.second.x)
		figures.insert(figures.end(), offense_.rod.figures_.begin(), offense_.rod.figures_.end());
	return figures;
}

std::vector<Gap> shrinkGaps(std::vector<Gap> gaps, Gap range)
{
	std::vector<Gap> ret;
	for (auto& g : gaps)
		if (!(g.start > range.end || g.end < range.start))
			ret.push_back(g);
	ret.front().start = range.start;
	ret.back().end = range.end;
	return ret;
}

std::vector<Gap> makeOnGoal(std::vector<Gap> gaps)
{
	return shrinkGaps(gaps, Gap( 0, GOAL_DIST ));
}

std::vector<Gap> Team::getRelevantGaps(std::vector<Shot> const& shots) const
{
	std::vector<Gap> ret;
	auto rect = getShotsArea(shots);
	std::vector<Figure> figures = getRelevantFigures(rect);
	std::sort(figures.begin(), figures.end(),
		[](Figure const& l, Figure const& r) {
		return l.y() < r.y();
	});
	for (Figure const& f : figures) {
		Gap gap;
		if (!ret.empty())
			gap.start = ret.back().end + FIGURE_FOOT;
		else
			gap.start = FIELD_MIN_Y;
		gap.end = f.y();
		ret.push_back(gap);
	}
	Gap last;
	if (!ret.empty())
		last.start = ret.back().end + FIGURE_FOOT;
	last.end = FIELD_MAX_Y;
	ret.push_back(last);
	if (rect.second.x < FIGURE_LEN + 2 * ROD_DIST)
	{
		// we only want to shoot on goal
		ret = makeOnGoal(ret);
	}
	else
		ret = shrinkGaps(ret, Gap( rect.first.y, rect.second.y ));
	return ret;
}

std::vector<Gap> Team::getRelevantFigures(std::vector<Shot> const& shots) const
{
	std::vector<Gap> ret;
	auto rect = getShotsArea(shots);
	std::vector<Figure> figures = getRelevantFigures(rect);
	std::sort(figures.begin(), figures.end(),
		[](Figure const& l, Figure const& r) {
		return l.y() < r.y();
	});
	for (auto f : figures)
		ret.push_back(Gap( f.y(), f.y() + FIGURE_LEN ));
	return ret;
}


std::ostream& operator<<(std::ostream& os, Shot const& shot)
{
	int BASE_LEN = 10;
	int BASE_LEN_ROW = 8 * BASE_LEN;
	int nbCols = (FIELD_MAX_Y - FIELD_MIN_Y) / BASE_LEN + 2;
	int nbRows = (FIELD_MAX_X - FIELD_MIN_X) / BASE_LEN_ROW + 2;

	return os;
}


std::ostream& operator<<(std::ostream& os, Team const& team)
{
	int BASE_LEN = 10;
	int BASE_LEN_ROW = 8 * BASE_LEN;
	int nbCols = (FIELD_MAX_Y - FIELD_MIN_Y) / BASE_LEN + 2;
	int nbRows = (FIELD_MAX_X - FIELD_MIN_X) / BASE_LEN_ROW + 2;

	std::string emptyLine;
	for (int j = 0; j < nbCols - 2; ++j)
		emptyLine += " ";

	for (int i = 0; i < nbRows; ++i)
	{
		os << " ";
		if (double(i)*BASE_LEN_ROW <= team.goalie_.rod.x() && team.goalie_.rod.x() <= double(i + 1)*BASE_LEN_ROW)
			os << team.goalie_.rod;
		else if (double(i)*BASE_LEN_ROW <= team.defense_.rod.x() && team.defense_.rod.x() <= double(i + 1)*BASE_LEN_ROW)
			os << team.defense_.rod;
		else if (double(i)*BASE_LEN_ROW <= team.midfield_.rod.x() && team.midfield_.rod.x() <= double(i + 1)*BASE_LEN_ROW)
			os << team.midfield_.rod;
		else if (double(i)*BASE_LEN_ROW <= team.offense_.rod.x() && team.offense_.rod.x() <= double(i + 1)*BASE_LEN_ROW)
			os << team.offense_.rod;
		else {
			os << emptyLine;
		}
		os << " \n";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, FoosballGame const& game)
{
	int BASE_LEN = 10;
	int BASE_LEN_ROW = 8 * BASE_LEN;
	int nbCols = (FIELD_MAX_Y - FIELD_MIN_Y) / BASE_LEN + 2;
	int nbRows = (FIELD_MAX_X - FIELD_MIN_X) / BASE_LEN_ROW + 2;
	for (int i = 0; i < nbRows; ++i) {
		os << "|";
		for (int j = 0; j < nbCols; ++j) {
			if ((i == 0 || i == nbRows - 1) && j >= 24 && j <= 44)
				os << "-";
			else 
				os << " ";
		}
		os << "|\n";
	}
	return os;
}
