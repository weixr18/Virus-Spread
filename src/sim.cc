
#include "game.h"
#include <stdio.h>
extern "C"
{
	__declspec(dllexport) bool sim(int days, int HOSPITAL_CAPACITY,
								   double P_MOVE,
								   double P_INFECT,
								   double P_VACCINATION,
								   double P_PROTECTION,
								   double P_OBSERVE);
}

bool sim(int days, int HOSPITAL_CAPACITY,
		 double P_MOVE,
		 double P_INFECT,
		 double P_VACCINATION,
		 double P_PROTECTION,
		 double P_OBSERVE)
{
	int steps = days * STEP_PER_DAY;
	printf("Total Steps: %d\n", steps);
	game = new Game(HOSPITAL_CAPACITY, P_MOVE,
					P_INFECT, P_VACCINATION, P_PROTECTION, P_OBSERVE);
	for (int i = 0; i < steps; i++)
	{
		game->Step();
	}
	game->SaveStep();
	return true;
}
