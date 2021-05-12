
#include "game.h"
#include <stdio.h>
extern "C"
{
	__declspec(dllexport) bool sim(int steps);
}

bool sim(int steps)
{
	printf("Total Steps: %d\n", steps);
	game = new Game();
	for (int i = 0; i < steps; i++)
	{
		game->Step();
	}
	game->SaveStep();
	return true;
}