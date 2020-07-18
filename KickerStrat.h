#pragma once
#include "Kicker.h"
#include "Evolution.h"
#include "CumMatrix.h"
#include <functional>
#include <cassert>
#include <map>

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

struct OffensiveData {
	CumVector<int> shootIfOpen;
	CumVector<int> shootIfClosed;
	int shootOpen = 100;
	std::vector<Shot> shots;
};

class OffensiveStrategy {
public:
	OffensiveStrategy(std::vector<Shot> shots, bool rand = false);
	double score(DefensiveStrategy const& chromosome) const;
	const Shot& chooseShot(DefensiveStrategy const& defense, int pos) const;

	void crossover(OffensiveStrategy const & other);
	void mutate();


	void stream(std::ostream& os, int intervals, double accuracy);

	OffensiveData gene_;
};

std::vector<Team> generateRelevantTeams(std::vector<Shot> shots);