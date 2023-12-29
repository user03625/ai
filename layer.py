class Layer:
    def __init(self):
        self.input = None
        slef.output = None

    def forward(self,input):
        raise NotImplementedError

    def backward(self,outputError,learningRate):
        raise NotImplementedError
        
