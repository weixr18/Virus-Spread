#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <random>

#include "grid.h"
#include "person.h"
#include "global.h"

constexpr int MAP_H = 700;
constexpr int MAP_W = 800;
constexpr int TOTAL_POPULATION = 5000;
const int INITIAL_INFECT_NUM = 50;
const int HOSPITAL_CAPACITY = 100;

const double P_MOVE = 0.1;
const double P_INFECT = 1.0;
const double P_VACCINATION = 0.0;
const double P_PROTECTION = 0.50;
const double P_CITY_DEAD = 0.1;
const double P_CITY_HEAL_NOT_DEAD = 2.0 / 9.0;
const double P_HOSPITAL_DEAD = 0.02;

class Game
{
public:
    std::default_random_engine random_generator_;
    std::bernoulli_distribution vaccinate_distribition_ = std::bernoulli_distribution(P_VACCINATION);
    std::bernoulli_distribution move_distribution_ = std::bernoulli_distribution(P_MOVE);
    std::normal_distribution<double> incubate_distribution_ = std::normal_distribution<double>(70, 20);
    std::normal_distribution<double> hospitalize_distribution_ = std::normal_distribution<double>(140, 10);
    std::bernoulli_distribution city_dead_distribution_ = std::bernoulli_distribution(P_CITY_DEAD);
    std::bernoulli_distribution city_not_dead_heal_distribution_ = std::bernoulli_distribution(P_CITY_HEAL_NOT_DEAD);
    std::bernoulli_distribution hospital_dead_distribution_ = std::bernoulli_distribution(P_HOSPITAL_DEAD);

    Grid hospital_ = Grid(kHospital);
    Grid tomb_ = Grid(kTomb);
    Grid heaven_ = Grid(kHeaven);
    int step_count_ = 0;
    Game();
    ~Game();

    void Step();
    void MoveStep();
    void SwitchStateStep();
    void SaveStep();
};

extern Game *game;
#endif //_GAME_H_