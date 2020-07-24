#pragma once
#include "Kicker.h"
#include "CumMatrix.h"
#include "Figure.h"
#include <functional>
#include <cassert>
#include <map>


class DefensiveStrategy;

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