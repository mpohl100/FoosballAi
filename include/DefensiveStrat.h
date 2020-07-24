#pragma once

#include "CumMatrix.h"
#include "Kicker.h"
#include <vector>

struct DefensiveData {
	std::vector<Team> teams;
	//std::vector<std::vector<int>> transition;
	CumMatrix<int> transition;
	int movesPerSecond = 0;
	int trans(int i) const;
};

class OffensiveStrategy;

class DefensiveStrategy{
public:
	DefensiveStrategy() = default;
	DefensiveStrategy(DefensiveStrategy const&) = default;
	DefensiveStrategy& operator=(DefensiveStrategy const&) = default;
	DefensiveStrategy(DefensiveStrategy&&) = default;
	DefensiveStrategy& operator=(DefensiveStrategy&&) = default;

	DefensiveStrategy(std::vector<Team> teams, bool rand = false);
	
	double score(OffensiveStrategy const& chromosome) const;
	void crossover(DefensiveStrategy const & other);
	void mutate();

	void stream(std::ostream& os, int intervals, double accuracy);

	DefensiveData gene_;
};

