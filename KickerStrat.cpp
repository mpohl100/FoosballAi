#include "KickerStrat.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <windows.h>


int fetchPosition(CumVector<int> const& vec)
{
	int randNb = fetchRand(0, vec.cum());
	int candidateN = vec.cum();
	int candidateI = -1;
	int maxI = 0;
	int maxN = 0;
	int currProb = 0;
	for (int j = 0; j < vec.size(); j++)
	{
		int prob = vec[j] + currProb;
		if (prob >= randNb && prob < candidateN) {
			candidateI = j;
			candidateN = prob;
		}
		if (prob > maxN) {
			maxN = prob;
			maxI = j;
		}
		currProb += vec[j];
	}
	return candidateI > -1 ? candidateI : maxI;
}


double play(OffensiveStrategy const& offensive, DefensiveStrategy const& defensive)
{
	const int range_from = 0;
	const int range_to = 100;
	static std::random_device                  rand_dev;
	static std::mt19937                        generator(rand_dev());
	static std::uniform_int_distribution<int>  distr(range_from, range_to);

	const int randomShots = 500;
	double fitness = 0.0;
	DefensiveData const& gene = defensive.gene_;
	for(int i = 0; i < randomShots; ++i)
	{
		int pos = fetchPosition(gene.transition.cum());
		int randIdx = int(double(distr(rand_dev)) / range_to * offensive.gene_.shots.size());
		if (randIdx >= offensive.gene_.shots.size())
			randIdx = offensive.gene_.shots.size() - 1;
		Shot const& shot = offensive.chooseShot(defensive, pos);
		pos = gene.trans(pos);
		double fit = gene.teams[pos].touchesShot(shot);
		fitness += fit;
	}
	return fitness / randomShots;
}

int DefensiveData::trans(int i) const
{
	int doMove = fetchRand<0, 10>();
	if (doMove > movesPerSecond)
		return i;
	return fetchPosition(transition[i]);
}

CumVector<int> generateRandomRowThresholds(int sum, int positions)
{
	CumVector<int> ret(positions);
	std::vector<int> indeces;
	indeces.reserve(positions);
	for (int i = 0; i < positions; ++i)
		indeces.push_back(i);
	std::random_shuffle(indeces.begin(), indeces.end());
	int currMin = 0;
	for (int i : indeces)
	{
		if (currMin < sum) {
			ret.set(i, fetchRand(0, sum-currMin));
			currMin = ret[i];
		}
		else
			ret.set(i, currMin);
	}
	return ret;
}

void normalize(std::vector<int>& row, int targetSum)
{
	int maxVal = *std::max_element(row.begin(), row.end());
	double multi = double(targetSum) / double(maxVal);
	for (int& i : row)
		i = int(double(i)*multi);
}

void generateRandomColThresholds(CumMatrix<int>& mat, int col)
{
	assert(col < mat.size());
	for (int i = 0; i < mat.size(); ++i)
	{
		int randProb = fetchRand(0, mat[i].cum());
		for (int j = 0; j < mat.size(); j++)
			mat.set(i, j, randProb);
	}
}

DefensiveStrategy::DefensiveStrategy(std::vector<Team> teams, bool rand)
{
	gene_.teams = teams;
	int N = gene_.teams.size();
	gene_.transition = CumMatrix<int>(N);
	if (rand) {
		for (int i = 0; i < teams.size(); ++i)
			gene_.transition.set(i, generateRandomRowThresholds(100000, teams.size()));
		gene_.movesPerSecond = fetchRand<1, 5>();
	}
	else
	{
		gene_.movesPerSecond = 4;
		for (int i = 0; i < teams.size(); ++i) {
			std::vector<int> count;
			for (int i = 0; i < teams.size(); ++i)
				count.push_back(100000 / teams.size());
			//normalize(count, 100000);
			gene_.transition.set(i, count);
		}
	}
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
		for (int i = gene_.teams.size() / 2; i < gene_.teams.size(); ++i)
			gene_.transition.set(i, other.gene_.transition[i]);
	}
	else {
		for (int i = 0; i <= gene_.teams.size() / 2; ++i)
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
		Sleep(250);
	}
}

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
	double len = GOAL_DIST - Ball::perimeter(0);
	if (chooseStrat < gene_.shootOpen) {
		std::vector<Gap> gaps = team.getRelevantGaps(gene_.shots);
		std::sort(gaps.begin(), gaps.end(), [](auto l, auto r) {return l.end - l.start > r.end - r.start; });
		int randGapIdx = fetchPosition(gene_.shootIfOpen);
		if (randGapIdx >= gaps.size())
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
		int halfBall = Ball::perimeter(0);
		int randGapIdx = fetchPosition(gene_.shootIfClosed);
		if (randGapIdx >= gaps.size())
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
	int pos = 0;
	for (int i = 0; i < intervals; ++i)
	{
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		std::cout << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy * 100;
		Sleep(250);
	}

}

std::vector<Team> getRelevantTeamPositions(std::pair<Point, Point> area, double dist = 10.0)
{
	std::vector<Team> ret;
	Team dummy;
	std::vector<std::vector<double>> all_y(4,std::vector<double>(1)); // mindestens ein Eintrag muss vorhanden sein
	if (area.first.x <= dummy.goalie_.rod.x() && dummy.goalie_.rod.x() <= area.second.x)
		for (double d = dummy.goalie_.rod.range_y().first; d <= dummy.goalie_.rod.range_y().second; d += dist)
			all_y[0].push_back(d);

	if (area.first.x <= dummy.defense_.rod.x() && dummy.defense_.rod.x() <= area.second.x)
		for (double d = dummy.defense_.rod.range_y().first; d <= dummy.defense_.rod.range_y().second; d += dist)
			all_y[1].push_back(d);

	if (area.first.x <= dummy.midfield_.rod.x() && dummy.midfield_.rod.x() <= area.second.x)
		for (double d = dummy.midfield_.rod.range_y().first; d <= dummy.midfield_.rod.range_y().second; d += dist)
			all_y[2].push_back(d);

	if (area.first.x <= dummy.offense_.rod.x() && dummy.offense_.rod.x() <= area.second.x)
		for (double d = dummy.offense_.rod.range_y().first; d <= dummy.offense_.rod.range_y().second; d += dist)
			all_y[3].push_back(d);

	for(double goalie : all_y[0])
		for (double defense : all_y[1])
			for(double midfield : all_y[2])
				for (double offense : all_y[3]) {
					Team t;
					t.goalie_.rod.move_y_to(goalie);
					t.defense_.rod.move_y_to(defense);
					t.midfield_.rod.move_y_to(midfield);
					t.offense_.rod.move_y_to(offense);
					ret.push_back(t);
				}
	return ret;
}


std::vector<Team> generateRelevantTeams(std::vector<Shot> shots)
{
	std::vector<Team> ret;
	if (shots.empty())
		return ret;
	auto rect = getShotsArea(shots);
	return getRelevantTeamPositions(rect);
}
