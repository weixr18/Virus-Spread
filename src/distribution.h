#ifndef _DISTRIBUTION_H
#define _DISTRIBUTION_H

#include <random>
#include "utils.h"
#include "definations.h"

const double P_CITY_DEAD = 0.1;
const double P_CITY_HEAL_NOT_DEAD = 0.2 / (1-P_CITY_DEAD);
const double P_HOSPITAL_DEAD = 0.02;


// domitory, canteen, playground, teachbuilding
const double M_TRANSFER[4][4] = {
    0.2, 0.4, 0.3, 0.1,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.2, 0.1,
    0.1, 0.3, 0.2, 0.4,
};

// domitory, canteen, playground, teachbuilding
const int MEAN_STAY_TIME[4] = {
    5, 1, 2, 3
};
const int STAY_TIME_STD = 2;

const Point CENTER_DOMITORY = {200,600};
const Point CENTER_CANTEEN = {600,600};
const Point CENTER_PLAYGROUND = {200,200};
const Point CENTER_TEACHBUILDING = {600,200};
const Point AREA_CENTERS[4] = {
    CENTER_DOMITORY,
    CENTER_CANTEEN,
    CENTER_PLAYGROUND,
    CENTER_TEACHBUILDING,
};
const int AREA_STDS[4] = {
    70,70,70,70
};


class Distribution {
public:
    const double P_MOVE_;
    const double P_INFECT_;
    const double P_VACCINATION_;
    const double P_PROTECTION_;
    const double P_OBSERVE_;

    std::default_random_engine random_generator_;
    std::bernoulli_distribution vaccinate_distribition_ = std::bernoulli_distribution(P_VACCINATION_);
    //std::bernoulli_distribution move_distribution_ = std::bernoulli_distribution(P_MOVE_);
    std::normal_distribution<double> incubate_distribution_ = std::normal_distribution<double>(7*STEP_PER_DAY, 2*STEP_PER_DAY);
    std::normal_distribution<double> hospitalize_distribution_ = std::normal_distribution<double>(14*STEP_PER_DAY, 2*STEP_PER_DAY);
    std::bernoulli_distribution city_dead_distribution_ = std::bernoulli_distribution(P_CITY_DEAD);
    std::bernoulli_distribution city_not_dead_heal_distribution_ = std::bernoulli_distribution(P_CITY_HEAL_NOT_DEAD);
    std::bernoulli_distribution hospital_dead_distribution_ = std::bernoulli_distribution(P_HOSPITAL_DEAD);
    std::bernoulli_distribution observe_distribution_ = std::bernoulli_distribution(P_OBSERVE_);
    std::uniform_real_distribution<> uniform_distribution_ = std::uniform_real_distribution<>();
    std::normal_distribution<double> init_x_distribution_ = std::normal_distribution<double>(AREA_CENTERS[0].x_, AREA_STDS[0]);
    std::normal_distribution<double> init_y_distribution_ = std::normal_distribution<double>(AREA_CENTERS[0].y_, AREA_STDS[0]);
    std::normal_distribution<double> stay_time_distributions_[4] = {
        std::normal_distribution<double>(MEAN_STAY_TIME[0], STAY_TIME_STD),
        std::normal_distribution<double>(MEAN_STAY_TIME[1], STAY_TIME_STD),
        std::normal_distribution<double>(MEAN_STAY_TIME[2], STAY_TIME_STD),
        std::normal_distribution<double>(MEAN_STAY_TIME[3], STAY_TIME_STD),
    };
    std::normal_distribution<double> next_x_distributions_[4] = {
        std::normal_distribution<double>(AREA_CENTERS[0].x_, AREA_STDS[0]),
        std::normal_distribution<double>(AREA_CENTERS[1].x_, AREA_STDS[1]),
        std::normal_distribution<double>(AREA_CENTERS[2].x_, AREA_STDS[2]),
        std::normal_distribution<double>(AREA_CENTERS[3].x_, AREA_STDS[3]),
    };
    std::normal_distribution<double> next_y_distributions_[4] = {
        std::normal_distribution<double>(AREA_CENTERS[0].y_, AREA_STDS[0]),
        std::normal_distribution<double>(AREA_CENTERS[1].y_, AREA_STDS[1]),
        std::normal_distribution<double>(AREA_CENTERS[2].y_, AREA_STDS[2]),
        std::normal_distribution<double>(AREA_CENTERS[3].y_, AREA_STDS[3]),
    };


    Distribution(
         const double P_MOVE,
         const double P_INFECT,
         const double P_VACCINATION,
         const double P_PROTECTION ,
         const double P_OBSERVE);

    bool IsVaccinated();
    bool IsInfected(int ill_count, double vaccinated);
    bool IsObserved();
    bool IsCityPersonDead();
    bool IsCityPersonNotDeadButHealed();
    bool IsHospitalPersonDead();
    int GetInitX();
    int GetInitY();
    int GetIncubateTime();
    int GetHospitalizeTime();
    double GetUniform();
    int GetStayTime(AreaType next_area);
    Point GetNextPosition(int step_count, AreaType current_area);
};

#endif //_DISTRIBUTION_H
