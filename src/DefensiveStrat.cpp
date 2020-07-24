#include "DefensiveStrat.h"
#include "KickerStrat.h"
#include "StratCommon.h"

#include <iomanip>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


DefensiveStrategy::DefensiveStrategy(std::vector<Team> teams, bool rand)
{
	gene_.teams = teams;
	int N = gene_.teams.size();
	gene_.transition = CumMatrix<int>(N);
	if (rand) {
		for (int i = 0; i < int(teams.size()); ++i)
			gene_.transition.set(i, generateRandomRowThresholds(100000, teams.size()));
		gene_.movesPerSecond = fetchRand<1, 5>();
	}
	else
	{
		gene_.movesPerSecond = 4;
		for (size_t i = 0; i < teams.size(); ++i) {
			std::vector<int> count;
			for (size_t i = 0; i < teams.size(); ++i)
				count.push_back(100000 / teams.size());
			//normalize(count, 100000);
			gene_.transition.set(i, count);
		}
	}
}

int DefensiveData::trans(int i) const
{
	int doMove = fetchRand<0, 10>();
	if (doMove > movesPerSecond)
		return i;
	return fetchPosition(transition[i]);
}

double DefensiveStrategy::score(OffensiveStrategy const& chromosome) const
{
	return play(chromosome, *this);
}

void DefensiveStrategy::crossover(DefensiveStrategy const & other)
{
	int coinflip = fetchRand<0, 1>();
	if (coinflip == 0)
	{
		for (size_t i = gene_.teams.size() / 2; i < gene_.teams.size(); ++i)
			gene_.transition.set(i, other.gene_.transition[i]);
	}
	else {
		for (size_t i = 0; i <= gene_.teams.size() / 2; ++i)
			gene_.transition.set(i, other.gene_.transition[i]);
	}
}

void DefensiveStrategy::mutate()
{
	int nbMutations = 1;
	int alterRow = fetchRand<0, 1>();
	for (int j = 0; j < nbMutations; ++j)
	{
		int randomRow = fetchRand(0, gene_.teams.size() - 1);
		if (alterRow)
			gene_.transition.set(randomRow, generateRandomRowThresholds(100000, gene_.teams.size()));
		else
			generateRandomColThresholds(gene_.transition, randomRow);
	}
	int alterVelo = fetchRand<0, 5>();
	if (alterVelo == 0)
		gene_.movesPerSecond = fetchRand<1, 3>();
}

void DefensiveStrategy::stream(std::ostream& os, int intervals, double accuracy)
{
	int pos = 0;
	for (int i = 0; i < intervals; ++i)
	{
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		pos = gene_.trans(pos);
		std::cout << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy * 100 
			<< " % Velocity: " << gene_.movesPerSecond << " moves/sec\n";
		os << gene_.teams[pos] << std::endl;
		std::this_thread::sleep_for (std::chrono::milliseconds(250));
	}
}

