import numpy as np


def convolve(a,b):
    shape = np.add(a.shape,b.shape)
    shape[0]=1
    c = np.zeros(shape)
    print(c.shape)
    b = np.flip(b)
    for i in range(a.size+b.size)
        c[i] = np.dot(a[:i],b[i:]







a = np.random.rand(1,1000)
b = np.random.rand(1,1000)


convolve(a,b)
