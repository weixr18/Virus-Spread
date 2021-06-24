#include "distribution.h"

Distribution::Distribution(
         const double P_MOVE,
         const double P_INFECT,
         const double P_VACCINATION,
         const double P_PROTECTION ,
         const double P_OBSERVE):P_MOVE_(P_MOVE), P_INFECT_(P_INFECT),
                              P_VACCINATION_(P_VACCINATION),
                              P_PROTECTION_(P_PROTECTION),
                              P_OBSERVE_(P_OBSERVE)
         {
         }

bool Distribution::IsVaccinated() {
    return vaccinate_distribition_(random_generator_);
}


bool Distribution::IsInfected(int ill_count, double vaccinated){
    double p_single_infect;
    if (vaccinated)
    {
        p_single_infect = P_INFECT_ * (1 - (P_PROTECTION_));
    }
    else
    {
        p_single_infect = P_INFECT_;
    }
    double p_infect = 1.0 - std::pow((1 - p_single_infect), ill_count);
    std::bernoulli_distribution infect_dist(p_infect);
    return infect_dist(random_generator_);
}

bool Distribution::IsObserved(){
    return observe_distribution_(random_generator_);
}

bool Distribution::IsCityPersonDead(){
    return city_dead_distribution_(random_generator_);
}

bool Distribution::IsCityPersonNotDeadButHealed(){
    return city_not_dead_heal_distribution_(random_generator_);
}

bool Distribution::IsHospitalPersonDead(){
    return hospital_dead_distribution_(random_generator_);
}

int Distribution::GetInitX() {
    return (int)init_x_distribution_(random_generator_);
}

int Distribution::GetInitY() {
    return (int)init_y_distribution_(random_generator_);
}

int Distribution::GetIncubateTime() {
    return (int)incubate_distribution_(random_generator_);
}

int Distribution::GetHospitalizeTime() {
    return (int)hospitalize_distribution_(random_generator_);
}

double Distribution::GetUniform(){
    return uniform_distribution_(random_generator_);
}

int Distribution::GetStayTime(AreaType next_area){
    int stay_time = (int) stay_time_distributions_[(size_t)next_area](random_generator_);
    return (stay_time > 0)? stay_time : 1;
}

Point Distribution::GetNextPosition(int step_count, AreaType current_area){
    AreaType target_area;
    if(step_count % STEP_PER_DAY == NIGHT_FALL_STEP){
        target_area = kDomitory;
    }
    else{
        double samp = GetUniform();
        const double *p = M_TRANSFER[((size_t)current_area)];
        for(int i = 0; i < 4; i++){
            if((samp -= p[i]) < 0){
                target_area = (AreaType)i;
            }
        }
        target_area = kDomitory;
    }
    // todo
    int i_target_area = (int) target_area;

    int next_x, next_y;
    do{
        next_x = (int) next_x_distributions_[i_target_area](random_generator_);
        next_y = (int) next_y_distributions_[i_target_area](random_generator_);
    }while(
        next_x < 0 || next_x >= MAP_H || next_y < 0 || next_y > MAP_W
    );

    return Point(next_x,next_y);
}

