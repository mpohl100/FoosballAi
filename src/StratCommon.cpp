#include "StratCommon.h"
#include "OffensiveStrat.h"
#include "DefensiveStrat.h"
#include "Kicker.h"

#include <algorithm>

int fetchPosition(CumVector<int> const& vec)
{
	int randNb = fetchRand(0, vec.cum());
	int candidateN = vec.cum();
	int candidateI = -1;
	int maxI = 0;
	int maxN = 0;
	int currProb = 0;
	for (int j = 0; j < int(vec.size()); j++)
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
		if (randIdx >= int(offensive.gene_.shots.size()))
			randIdx = offensive.gene_.shots.size() - 1;
		Shot const& shot = offensive.chooseShot(defensive, pos);
		pos = gene.trans(pos);
		double fit = gene.teams[pos].touchesShot(shot);
		fitness += fit;
	}
	return fitness / randomShots;
}


CumVector<int> generateRandomRowThresholds(int sum, int positions)
{
	std::vector<int> randomIntegers;
	int currentMax = sum;
	for(int i = 0; i < positions; i++)
	{
		int randInt = fetchRand(0, currentMax);
		randomIntegers.push_back(randInt);
		currentMax -= randInt;
		if(currentMax <= 0) currentMax = 1;
	}
	std::random_shuffle(randomIntegers.begin(), randomIntegers.end());
	return CumVector<int>(randomIntegers);
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
	assert(col < int(mat.size()));
	for (size_t i = 0; i < mat.size(); ++i)
	{
		int randProb = fetchRand(0, mat[i].cum());
		for (size_t j = 0; j < mat.size(); j++)
			mat.set(i, j, randProb);
	}
}
