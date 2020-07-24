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

// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(Point p, Point q, Point r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}







Ball::Ball(double x, double y)
	: x_(ConstrainedPosition( FIELD_MIN_X, FIELD_MAX_X ))
	, y_(ConstrainedPosition( FIELD_MIN_Y, FIELD_MAX_Y ))
{
	x_.move(x);
	y_.move(y-FIELD_MIN_Y);
}

double Ball::x() const { return x_.pos(); }
double Ball::y() const { return y_.pos(); }

double Ball::perimeter(double rod_y)
{
	return 12.5;
}

bool operator<(Ball const& l, Ball const& r)
{
	if (l.x() != r.x())
		return l.x() < r.x();
	return l.y() < r.y();
}

Figure::Figure(ConstrainedPosition y, ConstrainedPosition x )
	: x_(x), y_(y)
{
	x_.move(FIGURE_LEN);
}

int Figure::touchesShot(Shot shot, double x) const
{
	Point p1( x + x_.pos(), y_.pos() );
	Point p2( x + x_.pos(), y_.pos() + length_ );
	double halfBall = Ball::perimeter(x_.pos());
	if (doIntersect(p1, p2, shot.start_, shot.end_)) return 1;
	else if (doIntersect(p1, p2, shot.start_ - halfBall, shot.end_ - halfBall)) return 1;
	else if (doIntersect(p1, p2, shot.start_ + halfBall, shot.end_ + halfBall)) return 1;
	else return 0;
}

void Figure::move_y(double y)
{
	y_.move(y);
}

void Figure::move_x(double x)
{
	x_.move(x);
}

double Figure::y() const { return y_.pos(); }
double Figure::x() const { return x_.pos(); }

std::pair<double, double> Figure::range_y() const
{
	return std::pair<double, double>( y_.min(), y_.max() );
}

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
