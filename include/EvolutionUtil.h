#pragma once

#include <map>
#include <vector>

template<class Chromosome, class Challenge>
std::multimap<double, const Chromosome*>
fitnessCalculation(std::vector<Chromosome> const& candidates, Challenge challenge)
{
	std::multimap<double, const Chromosome*> ret;
	for (auto& cand : candidates)
		ret.insert(std::make_pair(challenge.score(cand), &cand));
	return ret;
}

template<class Chromosome>
std::vector<const Chromosome*>
selectMatingPool(std::multimap<double, const Chromosome*> const& fitness, int sep = 2)
{
	std::vector<const Chromosome*> ret;
	int i = 0;
	for (auto it = fitness.rbegin(); it != fitness.rend(); ++it)
	{
		ret.push_back(it->second);
		if (++i > int(fitness.size()) / sep - 1)
			break;
	}
	return ret;
}

template<class Chromosome>
void
crossover(
	typename std::vector<Chromosome>::iterator parentsBeg,
	typename std::vector<Chromosome>::iterator parentsEnd,
	typename std::vector<Chromosome>::iterator offspringBeg,
	typename std::vector<Chromosome>::iterator offspringEnd
)
{
	int dadIdx = 0; // the winner gets to spread his genes everywhere
	for (; parentsBeg != parentsEnd; ++parentsBeg) {
		*offspringBeg = *parentsBeg;
		offspringBeg->crossover(*(parentsBeg + dadIdx));
		offspringBeg++;
		if (offspringBeg == offspringEnd)
			return;
	}
}

template<class Chromosome>
void randomMutation(
	typename std::vector<Chromosome>::iterator offspringBeg,
	typename std::vector<Chromosome>::iterator offspringEnd
)
{
	for (; offspringBeg != offspringEnd; ++offspringBeg)
		offspringBeg->mutate();
}

