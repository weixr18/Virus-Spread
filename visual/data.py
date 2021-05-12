import numpy as np
import pandas as pd


def load_data(file_name):
    df = pd.read_csv("./data/" + file_name, sep='\t', header=None)
    data = df.values
    data = np.reshape(data, [-1, 5000, 4])
    return data
