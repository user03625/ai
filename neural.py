from layer import Layer 
import numpy as np

class FCLayer(Layer):
    def __init__(self, inputSize,outputSize):
        self.weights = np.random.rand(inputSize,outputSize) - 0.5
        self.bias = np.random.rand(1,outputSize) - 0.5

    def forward(self,inputData):
        self.input = inputData
        self.output = np.dot(self.input,self.weights) + self.bias
        return self.output

    def backward(self,outputError, learningRate):
        inputError = np.dot(outputError, self.weights.T)
        weightsError = np.dot(self.input.T, outputError)

        self.weights-=learningRate*weightsError
        self.bias-=learningRate*outputError
        return inputError

class ActivationLayer(Layer):
    def __init__(self, activation, activationDerivative):
        self.activation = activation
        self.activationDerivative = activationDerivative

    def forward(self, inputData):
        self.input = inputData
        self.output = self.activation(self.input)
        return self.output

    def backward(self, outputError, learningRate):
        return self.activationDerivative(self.input) * outputError


class Network:
    def __init__(self):
        self.layers=[]
        self.loss = None
        self.lossDerivative = None

    def add(self,layer):
        self.layers.append(layer)

    def use(self, loss , lossDerivative):
        self.loss = loss    
        self.lossDerivative = lossDerivative

    def predict(self, inputData):
        samples = len(inputData)
        result = []

        for i in range(samples):
            output = inputData[i]
            for layer in self.layers:
                output = layer.forward(output)
            result.append(output)

        return result

    def fit(self, x, y, epochs, learningRate):
        samples = len(x)

        for i in range(epochs):
            err = 0
            for j in range(samples):
                output = x[j]
                for layer in self.layers:
                    output = layer.forward(output)
                err += self.loss(y[j],output)
                error = self.lossDerivative(y[j],output)
                for layer in reversed(self.layers):
                    error = layer.backward(error, learningRate)
            err /= samples
            print(f'epoch {i+1}/{epochs} error={err}')
