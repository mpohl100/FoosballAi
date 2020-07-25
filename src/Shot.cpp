#include "Shot.h"
	
Shot::Shot(Ball start, Ball end)
	: start_(Point( start.x(), start.y() ) )
    , end_(Point( end.x(), end.y() ) )
{}