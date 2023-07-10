import numpy as np
import matplotlib.pyplot as plt
import os

#####-Aufgabe 1-#####

#2.
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
ax1.set_xlabel("log_10(Iterationszahl)")
ax1.set_ylabel("$||u||$")
ax1.set_xscale('log')
ax1.grid()        
ax1.legend()            
plt.savefig('../figs/h1_comparison_plot_1', dpi = 250)
#3.

#Level 1
fig4, ax4 = plt.subplots(figsize = (5, 4.5), dpi = 250)
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
ax4.set_xlabel("log_10(Iterationszahl)")
ax4.set_ylabel("$||u||$")
ax4.set_xscale('log')
ax4.set_title("Level 1")
ax4.grid()        
ax4.legend()            
plt.savefig('../figs/h1_level1', dpi = 250)

#Level 2
fig5, ax5 = plt.subplots(figsize = (5, 4.5), dpi = 250)
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
ax5.set_xlabel("log_10(Iterationszahl)")
ax5.set_ylabel("$||u||$")
ax5.set_xscale('log')
ax5.set_title("Level 2")
ax5.grid()        
ax5.legend()            
plt.savefig('../figs/h1_level2', dpi = 250)

#Level 3
fig6, ax6 = plt.subplots(figsize = (5, 4.5), dpi = 250)
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
ax6.set_xlabel("log_10(Iterationszahl)")
ax6.set_ylabel("$||u||$")
ax6.set_xscale('log')
ax6.set_title("Level 3")
ax6.grid()        
ax6.legend()            
plt.savefig('../figs/h1_level3', dpi = 250)

#Level 4
fig7, ax7 = plt.subplots(figsize = (5, 4.5), dpi = 250)
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
ax7.set_xlabel("log_10(Iterationszahl)")
ax7.set_ylabel("$||u||$")
ax7.set_xscale('log')
ax7.set_title("Level 4")
ax7.grid()        
ax7.legend()            
plt.savefig('../figs/h1_level4', dpi = 250)

#Level 5
fig8, ax8 = plt.subplots(figsize = (5, 4.5), dpi = 250)
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
ax8.set_xlabel("log_10(Iterationszahl)")
ax8.set_ylabel("$||u||$")
ax8.set_xscale('log')
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

#####-Aufgabe 2-#####

#import data

res_norm_level1_n4 = np.loadtxt("res_norm_aufgabe2_multi=0_N=4")
x_res_norm_level1_n4 = res_norm_level1_n4[:, 0]
y_res_norm_level1_n4 = res_norm_level1_n4[:, 1]

res_norm_level1_n6 = np.loadtxt("res_norm_aufgabe2_multi=0_N=6")
x_res_norm_level1_n6 = res_norm_level1_n6[:, 0]
y_res_norm_level1_n6 = res_norm_level1_n6[:, 1]

res_norm_level1_n8 = np.loadtxt("res_norm_aufgabe2_multi=0_N=8")
x_res_norm_level1_n8 = res_norm_level1_n8[:, 0]
y_res_norm_level1_n8 = res_norm_level1_n8[:, 1]

res_norm_level1_n10 = np.loadtxt("res_norm_aufgabe2_multi=0_N=10")
x_res_norm_level1_n10 = res_norm_level1_n10[:, 0]
y_res_norm_level1_n10 = res_norm_level1_n10[:, 1]

res_norm_level1_n12 = np.loadtxt("res_norm_aufgabe2_multi=0_N=12")
x_res_norm_level1_n12 = res_norm_level1_n12[:, 0]
y_res_norm_level1_n12 = res_norm_level1_n12[:, 1]

#higher Levels
res_norm_level2_n4 = np.loadtxt("res_norm_aufgabe2_multi=1_N=4")
x_res_norm_level2_n4 = res_norm_level2_n4[:, 0]
y_res_norm_level2_n4 = res_norm_level2_n4[:, 1]

res_norm_level4_n6 = np.loadtxt("res_norm_aufgabe2_multi=1_N=6")
x_res_norm_level4_n6 = res_norm_level4_n6[:, 0]
y_res_norm_level4_n6 = res_norm_level4_n6[:, 1]

res_norm_level6_n8 = np.loadtxt("res_norm_aufgabe2_multi=1_N=8")
x_res_norm_level6_n8 = res_norm_level6_n8[:, 0]
y_res_norm_level6_n8 = res_norm_level6_n8[:, 1]

res_norm_level8_n10 = np.loadtxt("res_norm_aufgabe2_multi=1_N=10")
x_res_norm_level8_n10 = res_norm_level8_n10[:, 0]
y_res_norm_level8_n10 = res_norm_level8_n10[:, 1]

res_norm_level10_n12 = np.loadtxt("res_norm_aufgabe2_multi=1_N=12")
x_res_norm_level10_n12 = res_norm_level10_n12[:, 0]
y_res_norm_level10_n12 = res_norm_level10_n12[:, 1]

#3.
fig10, ax10 = plt.subplots(figsize = (10, 6), dpi = 250)
plt.plot(x_res_norm_level1_n4, y_res_norm_level1_n4, label = '$N = 4$')
plt.plot(x_res_norm_level1_n6, y_res_norm_level1_n6, label = '$N = 6$')
plt.plot(x_res_norm_level1_n8, y_res_norm_level1_n8, label = '$N = 8$')
plt.plot(x_res_norm_level1_n10, y_res_norm_level1_n10, label = '$N = 10$')
plt.plot(x_res_norm_level1_n12, y_res_norm_level1_n12, label = '$N = 12$')
ax10.set_xlabel("log_10(Iterationszahl)")
ax10.set_ylabel("$||res||$")
ax10.set_xscale('log')
ax10.grid()
ax10.legend()
plt.savefig('../figs/h2_N_comparison_1', dpi = 250)

#4.
convergence_vel = np.array([np.max(x_res_norm_level2_n4), np.max(x_res_norm_level4_n6), np.max(x_res_norm_level6_n8), np.max(x_res_norm_level8_n10), np.max(x_res_norm_level10_n12)])
N = np.array([4,6,8,10,12])
fig18, ax18 = plt.subplots(figsize = (10, 6), dpi = 250)
plt.plot(N, convergence_vel)
ax18.set_xlabel("N")
ax18.set_ylabel("maximale Iterationszahl")
ax18.grid()
plt.savefig('../figs/h2_last_plot', dpi = 250)