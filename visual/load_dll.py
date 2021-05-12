import ctypes
import os


def test_load_dll(name="./bin/test.dll"):
    lib = ctypes.cdll.LoadLibrary(name)
    print(lib.Add(1, 2))
    print(lib.Add(1000, 20000))
    pass


def load_dll(name="./bin/sim.dll"):
    os.environ['path'] += ';./bin'
    lib = ctypes.cdll.LoadLibrary(name)
    return lib
