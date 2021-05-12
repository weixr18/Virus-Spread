# Virus-Spread

An interactive visualized simulation of virus spread.

## Dependencies

Our visualization script requires pyQt5 and other third-party libraries:

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

**Note:** You may need to move C++ standard library DLLs into \bin.

## Structure

+ bin\\: executables and dynamic libraries
+ data\\: simulation results
+ document\\: problem description(in Chinese)
+ report\\: summary of results
+ src\\: C++ codes, implement of simulation program
+ visual\\: python codes, interactive visualization of results
