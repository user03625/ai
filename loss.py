import numpy as np

def mse(y,pred):
    return np.mean(np.power(y-pred,2))

def mseDerivative(y,pred):
    return 2*(pred-y)/y.size

