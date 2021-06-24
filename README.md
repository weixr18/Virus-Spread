# Virus-Spread

An interactive visualized simulation of virus spread.

## Dependencies

Our visualization script requires Python package pyQt5 and other third-party libraries:

+ numpy\=\=1.20.3
+ pandas\=\=1.2.4
+ PyQt5\=\=5.15.4
+ PyQt5-Qt5\=\=5.15.2
+ PyQt5-sip\=\=12.9.0
+ six\=\=1.16.0

## Run

To run the executable:

```ps1
make all -j4
./bin/main
```

To visualize the simulation:

```ps1
make dll -j4
python3 main.py
```

p.s. The makefile is written for windows MinGW compiler. To compile in other environments, you need to modify it.

### About dynamic library loading on Windows

#### GCC Compiler

If you use the GCC compiler, you may encounter the following error:

```ps1
FileNotFoundError: Could not find module 'path\to\project\Virus-Spread\bin\sim.dll' (or one of its dependencies). Try using the full path with constructor syntax.
```

This is most likely because the system cannot find the dynamic link library for the C++ standard library that the program needs. In windows system, the dynamic link library path should be put into the environment variable PATH.

To solve this problem, there are two ways. First, put the \bin path of the gcc compiler you are using into the environment variable PATH. Second, put the following dlls from the \bin path of the gcc compiler into the \bin path of your project.

+ libstdc++-6.dll
+ libgcc_s_seh-1.dll
+ libwinpthread-1.dll

The python script responsible for loading the dll ([visual/utils.py](https://github.com/weixr18/Virus-Spread/blob/main/visual/utils.py)) will take the .\bin into the environment variable, so it is valid to do so.

#### MSVC Compiler

If you use Visual Studio, this would be a little bit more complicated.

Step 1, you need to create a new empty VS project (actually I can provide the project file, but I'm too lazy to install VS) and set its generation target as a dynamic link library (DLL).

Step 2, copy all the files under /src (except main.cpp & sim.cpp) to the VS project folder and index them properly in the project.

Step 3, the VS project should have pch.cpp and pch.h. We need to declare our interface function sim in pch.cpp in the following form:

```cpp
#include "game.h"
#include <stdio.h>
BOOL APIENTRY sim(int days, int HOSPITAL_CAPACITY,
	double P_MOVE,
	double P_INFECT,
	double P_VACCINATION,
	double P_PROTECTION,
	double P_OBSERVE)
{
	int steps = days * 12;
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
```

Step 4, generate the solution. Note that the target platform should have the same number of bits as your OS, and also the same number of bits as your python interpreter.

Step 5, copy the generated dll file (usually Debug/x64/DLL1.dll) to the /bin folder of the project. You can write a script for this step if you don't want to do it repeatedly, or modify main.py to let python do this automatically for you. :) 

Step 6: Modify the dll call method in ./visual/utils.h. Since we compiled it with the windows system compiler, we have to call it with the windll style.

```py
from ctypes import *
...
STEP_PER_DAY = 12
...
def load_dll(name="./bin/sim.dll"):
    lib = windll.LoadLibrary(name)
    lib.sim.argtypes = [c_int, c_int, c_double, c_double,
                        c_double, c_double, c_double]
    return lib

```

Finally, try to run main.py and good luck!

## Structure

+ \\bin: executables and dynamic libraries
+ \\data: simulation results
+ \\document: problem description(in Chinese)
+ \\report: summary of results
+ \\src: C++ codes, implement of simulation program
+ \\visual: python codes, interactive visualization of results

## Example

With the default parameters,

```ps1
HOSPITAL_CAPACITY = 100
P_MOVE = 0.9
P_INFECT = 0.6
P_VACCINATION = 0.4
P_PROTECTION = 0.8
P_OBSERVE = 0.0
```

the visualization looks like this

![1](report/run2.png)
