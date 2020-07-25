#pragma once

#include "FoosballMeasures.h"
#include "Shot.h"
#include "Rod.h"
#include <vector>
#include <string>

class FoosballField{
public:
    FoosballField(std::vector<Shot> shots,
                  std::vector<Rod> rods);
    void draw(std::ostream& os);
private:
    void drawRod(Rod const& rod);
    void drawShot(Shot const& shot);

    int BASE_LEN = 10;
	int BASE_LEN_ROW = 8 * BASE_LEN;
    std::vector<Shot> shots_;
    std::vector<Rod> rods_;
    std::vector<std::string> field_;
};