#pragma once

#include "Point.h"
#include "ConstrainedPosition.h"
#include "Ball.h"
#include "Figure.h"
#include "Rod.h"
#include "Shot.h"
#include "FoosballMeasures.h"
#include <random>

template<int range_from, int range_to>
int fetchRand()
{
	static std::random_device                  rand_dev;
	static std::mt19937                        generator(rand_dev());
	static std::uniform_int_distribution<int>  distr(range_from, range_to);
	return distr(rand_dev);
}

int fetchRand(int range_from, int range_to);

struct Goalie {
	Goalie() = default;
	Goalie(double y, double x);
	Rod rod = Rod({ Figure(ConstrainedPosition(0,GOAL_DIST - FIGURE_FOOT)) }, FIGURE_LEN);
};

struct Defense {
	Defense() = default;
	Defense(double y, double x = 0.0);

	Rod rod = Rod({ Figure(ConstrainedPosition(FIELD_MIN_Y+FIELD_FIGURE_OFFSET,
		GOAL_DIST-FIGURE_FOOT-FIELD_FIGURE_OFFSET)),
					Figure(ConstrainedPosition(FIELD_FIGURE_OFFSET,FIELD_MAX_Y-FIELD_FIGURE_OFFSET)) }
	, FIGURE_LEN + ROD_DIST);
};

struct Offense {
	Rod rod = Rod(
		{
			Figure(ConstrainedPosition(FIELD_MIN_Y+FIELD_FIGURE_OFFSET,0)),
			Figure(ConstrainedPosition(-FIGURE_FOOT,GOAL_DIST - FIGURE_FOOT)),
			Figure(ConstrainedPosition(GOAL_DIST-2*FIGURE_FOOT, FIELD_MAX_Y-FIELD_FIGURE_OFFSET))
		},
		FIGURE_LEN + ROD_DIST * 5);
};

struct Midfield {
	double FIGURE_DIST = ((FIELD_MAX_Y - FIELD_MIN_Y) - 2 * FIELD_FIGURE_OFFSET) / 5;
	Rod rod = Rod(
		{
			Figure(ConstrainedPosition(FIELD_MIN_Y + FIELD_FIGURE_OFFSET, FIELD_MIN_Y + FIELD_FIGURE_OFFSET + FIGURE_DIST)),
			Figure(ConstrainedPosition(FIELD_MIN_Y + FIELD_FIGURE_OFFSET + FIGURE_DIST, FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 2*FIGURE_DIST)),
			Figure(ConstrainedPosition(FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 2*FIGURE_DIST, FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 3 * FIGURE_DIST)),
			Figure(ConstrainedPosition(FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 3*FIGURE_DIST, FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 4 * FIGURE_DIST)),
			Figure(ConstrainedPosition(FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 4*FIGURE_DIST, FIELD_MIN_Y + FIELD_FIGURE_OFFSET + 5 * FIGURE_DIST)),
		},
		FIGURE_LEN + ROD_DIST * 3);
};

struct Gap {
	double start = 0;
	double end = 0;
	Gap(double s, double e) : start(s) , end(e) {}
	Gap() = default;
	Gap(Gap const&) = default;
	Gap& operator=(Gap const&) = default;
	Gap(Gap&&) = default;
	Gap& operator=(Gap&&) = default;
};

class Team {
public:
	Team(bool inversed)
		:inversed_(inversed)
	{}
	Team() = default;
	Team(Team const&) = default;
	Team& operator=(Team const&) = default;
	Team(Team&&) = default;
	Team& operator=(Team&&) = default;

	double touchesShot(Shot const& shot) const;
	std::vector<Figure> getRelevantFigures(std::pair<Point, Point> rect) const;
	std::vector<Gap> getRelevantGaps(std::vector<Shot> const& shots) const;
	std::vector<Gap> getRelevantFigures(std::vector<Shot> const& shots) const;


	Goalie goalie_;
	Defense defense_;
	Midfield midfield_;
	Offense offense_;
	bool inversed_ = false;
};

std::pair<Point, Point> getShotsArea(std::vector<Shot> const& shots);

class FoosballGame {
public:
	Team t1{ false };
	Team t2{ true };
	Ball ball;
};

std::ostream& operator<<(std::ostream& os, Team const& team);

std::ostream& operator<<(std::ostream& os, FoosballGame const& game);

std::vector<Team> generateRelevantTeams(std::vector<Shot> shots);