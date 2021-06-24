#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <random>

#include "distribution.h"
#include "grid.h"
#include "person.h"
#include "global.h"
#include "logger.h"


class Game
{
public:
    const int HOSPITAL_CAPACITY_;
    Distribution d_;
    Grid hospital_ = Grid(kHospital);
    Grid tomb_ = Grid(kTomb);

    int step_count_ = 0;
    Logger logger_;
    Game(const int HOSPITAL_CAPACITY = 500,
         const double P_MOVE = 0.5,
         const double P_INFECT = 0.1,
         const double P_VACCINATION = 0.9,
         const double P_PROTECTION = 0.50,
         const double P_OBSERVE = 0.05);
    ~Game();

    void Step();
    void MoveStep();
    void SwitchStateStep();
    void SaveStep();
};

extern Game *game;
#endif //_GAME_H_