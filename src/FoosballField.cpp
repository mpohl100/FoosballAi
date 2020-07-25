#include "FoosballField.h"
#include <algorithm>

FoosballField::FoosballField(std::vector<Shot> shots,
                             std::vector<Rod> rods)
: shots_(shots)
, rods_(rods)
{
	int nbCols = (FIELD_MAX_Y - FIELD_MIN_Y) / BASE_LEN + 2;
	int nbRows = (FIELD_MAX_X - FIELD_MIN_X) / BASE_LEN_ROW + 2;
    for(int i = 0; i < nbRows; ++i)
        field_.push_back(std::string(nbCols, ' '));
}

void FoosballField::drawRod( Rod const& rod)
{

}
    
void FoosballField::drawShot( Shot const& shot)
{
    int left_x = shot.start_.x;
    int left_y = shot.start_.y + FIELD_MIN_Y;
    int right_x = shot.end_.x;
    int right_y = shot.end_.y + FIELD_MIN_Y;
    // tauschen, falls die Gerade in die negative x-Richtung zeigt
    if(left_x > right_x)
    {
        std::swap(left_x, right_x);
        std::swap(left_y, right_y);
    }
        
    // Die Punkte skalieren
    left_x /= BASE_LEN_ROW;
    right_x /= BASE_LEN_ROW;
    left_y /= BASE_LEN;
    right_y /= BASE_LEN;
    // range checks
    if(left_x < 0 || left_x >= field_.size())
        throw std::runtime_error("left_x is out of range");    
    if(right_x < 0 || right_x >= field_.size())
        throw std::runtime_error("right_x is out of range");
    if(left_y < 0 || left_y >= field_[0].size())
        throw std::runtime_error("left_y is out of range");
    if(right_y < 0 || right_y >= field_[0].size())
        throw std::runtime_error("right_y is out of range");
    
    int delta_x = right_x - left_x;
    int delta_y = right_y - left_y;
    
    if(delta_x == 0)
    {
        if(left_y < right_y)
            for(int j = left_y; j <= right_y; ++j)
                field_[left_x][j] = 'O';
        else
            for(int j = left_y; j >= right_y; --j)
                field_[left_x][j] = 'O';
    }

    for(int i = left_x; i <= right_x; ++i)
    {
        int j = left_y + (right_y - left_y)*delta_y / delta_x;
        field_[i][j] = 'O';
    }    
}
    
void FoosballField::draw(std::ostream& os)
{

}