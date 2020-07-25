#pragma once

#include "FoosballMeasures.h"
#include "Shot.h"
#include "Rod.h"
#include <vector>
#include <string>

class FoosballField{
public:
    void draw(std::ostream& os);
private:
    void drawRod(Rod const& rod);
    void drawShot(Shot const& shot);
    std::vector<Shot> shots_;
    std::vector<Rod> rods_;
    std::vector<std::string> field_;
};