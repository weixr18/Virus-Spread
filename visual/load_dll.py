import ctypes
import os


def load_dll(name="./bin/sim.dll"):
    lib = ctypes.cdll.LoadLibrary(name)
    lib.sim.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_double,
                        ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return lib
