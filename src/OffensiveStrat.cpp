#include "OffensiveStrat.h"
#include "StratCommon.h"
#include "DefensiveStrat.h"
#include "FoosballField.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

OffensiveStrategy::OffensiveStrategy(std::vector<Shot> shots, bool rand)
{
	gene_.shots = shots;
	constexpr int numGapsOpen = 3;
	constexpr int numGapsClosed = 2;
	if (rand) {
		gene_.shootOpen = fetchRand<0,100>();
		gene_.shootIfOpen = generateRandomRowThresholds(100000, numGapsOpen);
		gene_.shootIfClosed = generateRandomRowThresholds(100000, numGapsClosed);
	}
	else {
		gene_.shootOpen = 50;
		gene_.shootIfOpen = CumVector<int>({90000,5000,5000});
		gene_.shootIfClosed = CumVector<int>({50000,50000});
	}
}

std::vector<int> getRandomIndeces(int N) {
	std::vector<int> ret;
	for (int i = 0; i < N; i++)
		ret.push_back(i);
	std::random_shuffle(ret.begin(), ret.end());
	return ret;
}

const Shot& OffensiveStrategy::chooseShot(DefensiveStrategy const& defense, int pos) const
{
	auto& team = defense.gene_.teams[pos];
	int chooseStrat = fetchRand<0, 100>();
	if (chooseStrat < gene_.shootOpen) {
		std::vector<Gap> gaps = team.getRelevantGaps(gene_.shots);
		std::sort(gaps.begin(), gaps.end(), 
				  [](Gap l, Gap r) {return l.end - l.start > r.end - r.start; });
		int randGapIdx = fetchPosition(gene_.shootIfOpen);
		if (randGapIdx >= int(gaps.size()))
			randGapIdx = 0;
		Gap randGap = gaps[randGapIdx];
		std::vector<int> indeces = getRandomIndeces(gene_.shots.size());
		int halfBall = Ball::perimeter(0);
		for (int i : indeces) {
			const Shot& shot = gene_.shots[i];
			if (randGap.start + halfBall + 1 <= shot.end_.y && shot.end_.y <= randGap.end - halfBall - 1)
				return shot;
		}
		int randShot = fetchRand(0, gene_.shots.size() - 1);
		return gene_.shots[randShot];
	}
	else {
		std::vector<Gap> gaps = team.getRelevantFigures(gene_.shots);
		std::vector<int> indeces = getRandomIndeces(gene_.shots.size());
		int randGapIdx = fetchPosition(gene_.shootIfClosed);
		if (randGapIdx >= int(gaps.size()))
			randGapIdx = 0;
		Gap randGap = gaps[randGapIdx];
		for (int i : indeces) {
			const Shot& shot = gene_.shots[i];
			if (randGap.start <= shot.end_.y && shot.end_.y <= randGap.end)
				return shot;
		}
		int randShot = fetchRand(0, gene_.shots.size() - 1);
		return gene_.shots[randShot];
	}
}


void OffensiveStrategy::crossover(OffensiveStrategy const & other)
{
	int probShootOpen = fetchRand<0, 1>();
	if (probShootOpen == 0)
		gene_.shootOpen = other.gene_.shootOpen;
	int probChooseOpen = fetchRand<0, 1>();
	if (probChooseOpen == 0)
		gene_.shootIfOpen = other.gene_.shootIfOpen;
	else
		gene_.shootIfClosed = other.gene_.shootIfOpen;
}

void OffensiveStrategy::mutate()
{
	int probAlter = fetchRand<0, 2>();
	if (probAlter == 0)
		gene_.shootOpen = fetchRand<0, 100>();
	else if (probAlter == 1)
		gene_.shootIfOpen = generateRandomRowThresholds(100000, gene_.shootIfOpen.size());
	else
		gene_.shootIfClosed = generateRandomRowThresholds(100000, gene_.shootIfClosed.size());
}

double OffensiveStrategy::score(DefensiveStrategy const& chromosome) const
{
	return play(*this, chromosome);
}

void OffensiveStrategy::stream(std::ostream& os, int intervals, double accuracy)
{
	for (int i = 0; i < intervals; ++i)
	{
		os << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		os << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy * 100;
		std::this_thread::sleep_for (std::chrono::milliseconds(250));
	}

}

