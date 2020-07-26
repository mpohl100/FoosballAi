#include "FoosballField.h"

int main()
{
    std::vector<Shot> star={
        Shot(Ball(600,100), Ball(600, 600)),
        Shot(Ball(600,100), Ball(600, -240)),
        Shot(Ball(600,100), Ball(1200, 100)),
        Shot(Ball(600,100), Ball(0, 100)),
        Shot(Ball(600,100), Ball(1200, 680)),
        Shot(Ball(600,100), Ball(0, 600)),
        Shot(Ball(600,100), Ball(1200, -240)),
        Shot(Ball(600,100), Ball(0, -240)),
    };
    FoosballField field(star, {});
    field.draw(std::cout); 
    return 0;
}