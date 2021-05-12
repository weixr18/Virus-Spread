import os

import numpy as np
import matplotlib.pyplot as plt

from .utils import load_dll, load_data

DLL = load_dll()

HOSPITAL_CAPACITY = 100
P_MOVE = 0.9
P_INFECT = 0.6
P_VACCINATION = 0.4
P_PROTECTION = 0.8

VARS = [HOSPITAL_CAPACITY, P_MOVE, P_INFECT,
        P_VACCINATION, P_PROTECTION]
VAR_NAMES = ["HOSPITAL_CAPACITY", "P_MOVE", "P_INFECT",
             "P_VACCINATION", "P_PROTECTION"]
var_names = ["hospital_capacity", "p_move", "p_infect",
             "p_vaccination", "p_protection"]


def get_file_name(params):
    HOSPITAL_CAPACITY, P_MOVE, P_INFECT, P_VACCINATION, P_PROTECTION = params
    str_ = "C{0:0>3d}_M{1:.2f}_I{2:.2f}_V{3:.2f}_P{4:.2f}.txt".format(
        HOSPITAL_CAPACITY,
        P_MOVE,
        P_INFECT,
        P_VACCINATION,
        P_PROTECTION,
    )
    return str_


def run_params(params):
    if not os.path.isfile('./data/'+get_file_name(params)):
        HOSPITAL_CAPACITY, P_MOVE, P_INFECT, P_VACCINATION, P_PROTECTION = params
        DLL.sim(30, HOSPITAL_CAPACITY, P_MOVE,
                P_INFECT, P_VACCINATION, P_PROTECTION)
    else:
        return


def show_data(x, data, title="", xlabel="x", ylabel="y"):
    fig, ax = plt.subplots()
    ax.plot(x, data[:, 0], label='healthy', color='green')
    ax.plot(x, data[:, 1], label='infected', color='yellow')
    ax.plot(x, data[:, 2], label='confirmed', color='red')
    ax.plot(x, data[:, 3], label='hospitalized', color='orange')
    ax.plot(x, data[:, 4], label='healed', color='blue')
    ax.plot(x, data[:, 5], label='dead', color='black')
    ax.set_title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    ax.legend()
    plt.show()
    pass


def study_on(variable=0, x=np.linspace(0, 1.0, num=11)):
    vars_ = VARS.copy()
    stats = []
    for i in x:
        vars_[variable] = i
        run_params(vars_)
        _, counts = load_data(get_file_name(vars_))
        last_counts = counts[:, 30]
        stats.append(last_counts)
    stats = np.array(stats)
    show_data(
        x, stats,
        title=VAR_NAMES[variable]+"'s effect",
        xlabel=var_names[variable],
        ylabel="number"
    )
    pass


def plot():
    for i in range(0, 5):
        if i == 0:
            study_on(i, list(range(0, 550, 50)))
        else:
            study_on(i)
    pass
