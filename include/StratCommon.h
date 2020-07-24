#pragma once

#include "CumMatrix.h"
#include <vector>

class DefensiveStrategy;
class OffensiveStrategy;

int fetchPosition(CumVector<int> const& vec);

double play(OffensiveStrategy const& offensive, DefensiveStrategy const& defensive);

CumVector<int> generateRandomRowThresholds(int sum, int positions);

void normalize(std::vector<int>& row, int targetSum);

void generateRandomColThresholds(CumMatrix<int>& mat, int col);

