import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("test.txt")
x = data[:,0]
y = data[:,1]
if (len(data[0,:]) == 3):
    global z 
    z = data[:,2]


plt.subplots()
plt.plot(x, y)
if (len(data[0,:]) == 3):
    plt.plot(x, z, linestyle="--")
plt.show()