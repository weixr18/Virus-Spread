#ifndef _GRID_H_
#define _GRID_H_

#include <vector>

#include "utils.h"
#include "global.h"

enum GridType
{
    kCity = 0,
    kHospital = 1,
    kTomb = 2,
    kHeaven = 3,
};

class Grid
{
public:
    Point position_;
    GridType type_;
    std::vector<Person *> persons_;
    std::vector<Grid *> neighbor_grids_;
    Grid();
    Grid(GridType type);
    ~Grid();
};

#endif //_GRID_H_