#include <iostream>

#include "game.h"

int main(int argc, char **argv)
{
    const int HOSPITAL_CAPACITY = 500;
    const double P_MOVE = 0.5;
    const double P_INFECT = 0.1;
    const double P_VACCINATION = 0.9;
    const double P_PROTECTION = 0.50;
    game = new Game(HOSPITAL_CAPACITY, P_MOVE,
                    P_INFECT, P_VACCINATION, P_PROTECTION);
    for (int i = 0; i < TOTAL_STEPS; i++)
    {
        game->Step();
    }
    game->SaveStep();
    return 0;
}