import ctypes
import os


def load_dll(name="./bin/sim.dll"):
    lib = ctypes.cdll.LoadLibrary(name)
    return lib
