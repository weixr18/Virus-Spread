
import ctypes
import os


import numpy as np
import pandas as pd


def load_data(file_name):
    df = pd.read_csv("./data/" + file_name, sep='\t', header=None)
    data = df.values
    data = np.reshape(data, [-1, 5000, 4])
    data = data[::10, :, :]
    counts = []
    for i in range(6):
        counts.append(np.sum(data[:, :, 1] == i, axis=1))
    counts = np.array(counts)
    return data, counts


def load_dll(name="./bin/sim.dll"):
    lib = ctypes.cdll.LoadLibrary(name)
    lib.sim.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_double,
                        ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return lib
