#include "FoosballField.h"

int main()
{
    std::vector<Shot> star={
        Shot(Ball(600,340), Ball(600, 600)),
        Shot(Ball(600,340), Ball(600, 0)),
        Shot(Ball(600,340), Ball(1200, 340)),
        Shot(Ball(600,340), Ball(0, 340)),
        Shot(Ball(600,340), Ball(1200, 600)),
        Shot(Ball(600,340), Ball(0, 600)),
        Shot(Ball(600,340), Ball(1200, 0)),
        Shot(Ball(600,340), Ball(0, 0)),
    };
    FoosballField field(star, {});
    field.draw(std::cout); 
    return 0;
}