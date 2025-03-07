import numpy as np


np.random.seed(0) 
x = np.array([1,2,3])
y = np.array([4,5,6])

new= np.concatenate([x,y])
print(new)