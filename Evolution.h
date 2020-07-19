#pragma once

#include "EvolutionUtil.h"
#include <iostream>
#include <algorithm>

// This library encapsulates evolutional learning through genetic algorithms 
// The library is header only and easy to use. 
// You have a type (the chromosome) whose values you want to optimize through evolutional learning
// and you have a challenge your type needs to master
// 

// These functions must be added to your chromosome type (the one you want to optimize)
//struct Chromosome {
//	void crossover(Chromosome const& other); make sure to randomly choose what data members should be taken from this or other
//	void mutate(); make sure to not randomly mutate too much of your chromosome type (only one data member at a time is recommended)
//};

// These functions must be added to your challenge type
//struct Challenge {
//	double score(Chromosome const& chromosome); the chromosome faces the challenge and its performance needs to be evaluated with a double (0 means bad, the higher the better the performance)
//};

template<class Chromosome, class Challenge>
std::vector<Chromosome>
evolution(
	std::vector<Chromosome> population, // feed with a few (20 recommended) random chromosomes
	int num_generations, // the number of generations to cross
	Challenge challenge, // the challenge 
	double& winningAccuracy, // the winning performance is an out parameter
	int log = 1 // logging level to see how far the algorithm progressed
)
{
	auto candidates = population;
	for (int i = 0; i < num_generations; ++i) {
		// let the chromosomes face the challenge
		std::multimap<double, const Chromosome*> fitness = fitnessCalculation(candidates, challenge);
		// logging
		if (log >= 1) {
			std::cout << "generation nr. " << i + 1 << " / " << num_generations << '\n';
			if (log >= 2) {
				for (auto& f : fitness)
					std::cout << "  fitness: " << f.first << '\n';
				std::cout << '\n';
			}
		}
		// half of the chromosomes are winners
		std::vector<const Chromosome*> winners = selectMatingPool(fitness, 2);
		// return the winner of the last generation
		if (i >= num_generations - 1)
		{
			std::vector<Chromosome> ret;
			for (auto w : winners)
				ret.push_back(*w);
			winningAccuracy = fitness.rbegin()->first;
			return ret;
		}
		// sort the chromosomes according to their fitness
		typename std::vector<Chromosome>::iterator sep = std::stable_partition(candidates.begin(), candidates.end(), 
			[&winners](Chromosome const& ch) {
			return std::find_if(winners.begin(), winners.end(),
				[&ch](const Chromosome* address) { return &ch == address; }) != winners.end();
		});
		// cross parents
		crossover<Chromosome>(candidates.begin(), sep, sep, candidates.end());
		// mutate children
		randomMutation<Chromosome>(sep, candidates.end());
	}
	return candidates;
}
