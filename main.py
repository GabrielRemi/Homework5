import numpy as np
import matplotlib.pyplot as plt
import os

########### AUFGABE 1 ##############################

os.chdir("results")
for i in range(5):
    fig, ax = plt.subplots()
    for file in os.listdir("."):
        if f'level{i+1}' in file:
            data = np.loadtxt(file)
            iterations = data[:, 0]
            u_norm = data[:, 1]
            ax.scatter(iterations, u_norm, label = file[13:])
    fig.savefig(f'../figs/level{i+1}', dpi = 200)