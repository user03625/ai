import numpy as np

from neural import Network, FCLayer, ActivationLayer
from activation import tanh, tanhDerivative
from loss import mse, mseDerivative


x = np.array([[[0,0]],[[0,1]],[[1,0]],[[1,1]]])
y = np.array([[[0]],[[1]],[[1]],[[0]]])

net = Network()

net.add(FCLayer(2,3))
net.add(ActivationLayer(tanh,tanhDerivative))
net.add(FCLayer(3,1))
net.add(ActivationLayer(tanh,tanhDerivative))

net.use(mse,mseDerivative)
net.fit(x,y,epochs=1000,learningRate=0.1)

out = net.predict(x)
print(out)

