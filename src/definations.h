#ifndef _DEFINATIONS_H
#define _DEFINATIONS_H

const int TOTAL_STEPS = 30*12;
const int STEP_PER_DAY = 12;
const int NIGHT_FALL_STEP = 9;
constexpr int MAP_H = 800;
constexpr int MAP_W = 800;
constexpr int TOTAL_POPULATION = 5000;
const int INITIAL_INFECT_NUM = 50;

enum AreaType
{
    kDomitory = 0,
    kCanteen = 1,
    kPlayground = 2,
    kTeachArea = 3,
};

#endif //_DEFINATIONS_H