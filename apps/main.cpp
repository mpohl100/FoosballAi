#include "Evolution.h"
#include "KickerStrat.h"
#include "DefensiveStrat.h"
#include "Kicker.h"

#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <iostream>
#include <random>
#include <cassert>
#include <functional>

std::vector<Shot> readInShots() {
	std::vector<Shot> ret;
	bool readIn = true;
	std::cout << "Please enter the shots the goalie should train to block\n";
	while (readIn) {
		double startingPointX = 0;
		double startingPointY = 0;
		char sep;
		std::cout << "The goal is at (0,0) and (0,205);\n";
		std::cout << "the goalie is at x: 73.75;\n";
		std::cout << "the defense is at x: 221.75;\n";
		std::cout << "the midfield is at x: 516.25;\n";
		std::cout << "the offense is at x: 811.25;\n";

		std::cout << "Please enter the starting point of the shot(NN,NN):" << std::endl;
		std::cin >> startingPointX;
		std::cin >> sep;
		std::cin >> startingPointY;
		double endingPointX = 0;
		double endingPointY = 0;
		std::cout << "Please enter the ending point of the shot(NN,NN):" << std::endl;
		std::cin >> endingPointX;
		std::cin >> sep;
		std::cin >> endingPointY;
		ret.push_back(Shot(Ball(startingPointX, startingPointY), Ball(endingPointX, endingPointY)));
		std::cout << "Enter another shot? (y|n):";
		char anotherShot;
		std::cin >> anotherShot;
		readIn = anotherShot == 'y';
	}
	return ret;
}

int main()
{
	std::cout << "Welcome to the Table Foosball training software!\n";
	std::cout << "What do you want to train? The offensive strategy (o) or the defensive strategy(d)?:";
	char strategy;
	std::cin >> strategy;
	std::vector<Shot> shots = { Shot(Ball(300,0), Ball(0,0)), 
								Shot(Ball(300,50), Ball(0,50)),
								Shot(Ball(300,100), Ball(0,100)),
								Shot(Ball(300,150), Ball(0,150)),
								Shot(Ball(300,200), Ball(0,200)),};
	std::vector<Team> teams = generateRelevantTeams(shots);
	std::cout << "print generated defense teams(" << teams.size() << ")? (y|n):";
	char printTeams = 'n';
	std::cin >> printTeams;
	if (printTeams == 'y')
		for (auto t : teams)
			std::cout << '\n' << t << '\n';
	int nbGenerations = 0;
	std::cout << "enter the nb of generations for the evolutional training:" << std::endl;
	std::cin >> nbGenerations;
	double accuracy = 0;
	if (strategy == 'd')
	{
		std::vector<DefensiveStrategy> population;
		for (int i = 0; i < 20; ++i)
			population.push_back(DefensiveStrategy(teams, true));
		OffensiveStrategy challenge(shots, false);
		auto winners = evolution(population, nbGenerations, challenge, accuracy, 2);
		winners[0].stream(std::cout, 1000, accuracy);
	}
	else {
		std::vector<OffensiveStrategy> population;
		for (int i = 0; i < 20; ++i)
			population.push_back(OffensiveStrategy(shots, true));
		DefensiveStrategy challenge(teams, true);
		auto winners = evolution(population, nbGenerations, challenge, accuracy, 2);
		winners[0].stream(std::cout, 1000, accuracy);
	}
	return 0;
}