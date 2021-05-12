#include <iostream>

#include "game.h"

Game *game;

int main(int argc, char **argv)
{
    game = new Game();
    for (int i = 0; i < TOTAL_STEPS; i++)
    {
        game->Step();
    }
    game->SaveStep();
    return 0;
}