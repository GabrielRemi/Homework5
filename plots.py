import numpy as np
import matplotlib.pyplot as plt
import os

#####-Aufgabe 1-#####

#1.
os.chdir("results")
fig1, ax1 = plt.subplots(figsize = (10, 6), dpi = 250)
data_k1 = np.loadtxt("unorm_aufgabe1_level1_k1")
iterations_k1 = data_k1[:, 0]
u_norm_k1 = data_k1[:,1]
plt.plot(iterations_k1, u_norm_k1, label = '$k = 1$')
for i in range(6):
    for file in os.listdir("."):
        if f'level1_k{21+20*i}' in file:
            data = np.loadtxt(file)
            iterations = data[:, 0]
            u_norm = data[:, 1]
            plt.plot(iterations, u_norm, label = f'$k = {21+20*i}$')            
ax1.set_xlabel("Iterationszahl")
ax1.set_ylabel("$||u||$")
ax1.grid()        
ax1.legend()            
plt.savefig('../figs/h1_comparison_plot_1', dpi = 250)

fig2, ax2 = plt.subplots(figsize = (10, 6), dpi = 250)
for i in range(6):
    for file in os.listdir("."):
        if f'level1_k{21+20*i}' in file:
            data = np.loadtxt(file)
            iterations = data[:, 0]
            u_norm = data[:, 1]
            plt.plot(iterations, u_norm, label = f'$k = {21+20*i}$')
ax2.set_xlabel("Iterationszahl")
ax2.set_ylabel("$||u||$")
ax2.set_xlim(1, 100)
ax2.grid()        
ax2.legend()            
plt.savefig('../figs/h1_comparison_plot_2', dpi = 250)

fig3, ax3 = plt.subplots(figsize = (10, 6), dpi = 250)
for i in range(6):
    for file in os.listdir("."):
        if f'level1_k{21+20*i}' in file:
            data = np.loadtxt(file)
            iterations = data[:, 0]
            u_norm = data[:, 1]
            plt.plot(iterations, u_norm, label = f'$k = {21+20*i}$')
ax3.set_xlabel("Iterationszahl")
ax3.set_ylabel("$||u||$")
ax3.set_xlim(1, 20)
ax3.grid()        
ax3.legend()            
plt.savefig('../figs/h1_comparison_plot_3', dpi = 250)

#2.

#Level 1
fig4, ax4 = plt.subplots(figsize = (10, 6), dpi = 250)
data_level1_k1 = np.loadtxt("unorm_aufgabe1_level1_k1")
iterations_level1_k1 = data_level1_k1[:, 0]
u_norm_level1_k1 = data_level1_k1[:,1]
plt.plot(iterations_level1_k1, u_norm_level1_k1, label = '$k = 1$')
for i in range(3):
    for file in os.listdir("."):
        if f'level1_k{41+40*i}' in file:
            data_level1 = np.loadtxt(file)
            iterations_level1 = data_level1[:, 0]
            u_norm_level1 = data_level1[:, 1]
            plt.plot(iterations_level1, u_norm_level1, label = f'$k = {41+20*i}$')            
ax4.set_xlabel("Iterationszahl")
ax4.set_ylabel("$||u||$")
ax4.set_title("Level 1")
ax4.grid()        
ax4.legend()            
plt.savefig('../figs/h1_level1', dpi = 250)

#Level 2
fig5, ax5 = plt.subplots(figsize = (10, 6), dpi = 250)
data_level2_k1 = np.loadtxt("unorm_aufgabe1_level2_k1")
iterations_level2_k1 = data_level2_k1[:, 0]
u_norm_level2_k1 = data_level2_k1[:,1]
plt.plot(iterations_level2_k1, u_norm_level2_k1, label = '$k = 1$')
for i in range(3):
    for file in os.listdir("."):
        if f'level2_k{41+40*i}' in file:
            data_level2 = np.loadtxt(file)
            iterations_level2 = data_level2[:, 0]
            u_norm_level2 = data_level2[:, 1]
            plt.plot(iterations_level2, u_norm_level2, label = f'$k = {41+20*i}$')            
ax5.set_xlabel("Iterationszahl")
ax5.set_ylabel("$||u||$")
ax5.set_title("Level 2")
ax5.grid()        
ax5.legend()            
plt.savefig('../figs/h1_level2', dpi = 250)

#Level 3
fig6, ax6 = plt.subplots(figsize = (10, 6), dpi = 250)
data_level3_k1 = np.loadtxt("unorm_aufgabe1_level3_k1")
iterations_level3_k1 = data_level3_k1[:, 0]
u_norm_level3_k1 = data_level3_k1[:,1]
plt.plot(iterations_level3_k1, u_norm_level3_k1, label = '$k = 1$')
for i in range(3):
    for file in os.listdir("."):
        if f'level3_k{41+40*i}' in file:
            data_level3 = np.loadtxt(file)
            iterations_level3 = data_level3[:, 0]
            u_norm_level3 = data_level3[:, 1]
            plt.plot(iterations_level3, u_norm_level3, label = f'$k = {41+20*i}$')            
ax6.set_xlabel("Iterationszahl")
ax6.set_ylabel("$||u||$")
ax6.set_title("Level 3")
ax6.grid()        
ax6.legend()            
plt.savefig('../figs/h1_level3', dpi = 250)

#Level 4
fig7, ax7 = plt.subplots(figsize = (10, 6), dpi = 250)
data_level4_k1 = np.loadtxt("unorm_aufgabe1_level4_k1")
iterations_level4_k1 = data_level4_k1[:, 0]
u_norm_level4_k1 = data_level4_k1[:,1]
plt.plot(iterations_level4_k1, u_norm_level4_k1, label = '$k = 1$')
for i in range(3):
    for file in os.listdir("."):
        if f'level4_k{41+40*i}' in file:
            data_level4 = np.loadtxt(file)
            iterations_level4 = data_level4[:, 0]
            u_norm_level4 = data_level4[:, 1]
            plt.plot(iterations_level4, u_norm_level4, label = f'$k = {41+20*i}$')            
ax7.set_xlabel("Iterationszahl")
ax7.set_ylabel("$||u||$")
ax7.set_title("Level 4")
ax7.grid()        
ax7.legend()            
plt.savefig('../figs/h1_level4', dpi = 250)

#Level 5
fig8, ax8 = plt.subplots(figsize = (10, 6), dpi = 250)
data_level5_k1 = np.loadtxt("unorm_aufgabe1_level5_k1")
iterations_level5_k1 = data_level5_k1[:, 0]
u_norm_level5_k1 = data_level5_k1[:,1]
plt.plot(iterations_level5_k1, u_norm_level5_k1, label = '$k = 1$')
for i in range(3):
    for file in os.listdir("."):
        if f'level5_k{41+40*i}' in file:
            data_level5 = np.loadtxt(file)
            iterations_level5 = data_level5[:, 0]
            u_norm_level5 = data_level5[:, 1]
            plt.plot(iterations_level5, u_norm_level5, label = f'$k = {41+20*i}$')            
ax8.set_xlabel("Iterationszahl")
ax8.set_ylabel("$||u||$")
ax8.set_title("Level 5")
ax8.grid()        
ax8.legend()            
plt.savefig('../figs/h1_level5', dpi = 250)

#Vergleich aller 5 Levels in einem Plot
fig9, ax9 = plt.subplots(figsize = (10, 6), dpi = 250)
plt.plot(iterations_level1_k1, u_norm_level1_k1, label = 'Level 1')
plt.plot(iterations_level2_k1, u_norm_level2_k1, label = 'Level 2')
plt.plot(iterations_level3_k1, u_norm_level3_k1, label = 'Level 3')
plt.plot(iterations_level4_k1, u_norm_level4_k1, label = 'Level 4')
plt.plot(iterations_level5_k1, u_norm_level5_k1, label = 'Level 5')
ax9.set_xlabel("Iterationszahl")
ax9.set_ylabel("$||u||$")
ax9.grid()
ax9.legend()
plt.savefig('../figs/h1_level_comparison', dpi = 250)