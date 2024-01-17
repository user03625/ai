import matplotlib.pyplot as plt
import numpy as np

class LiniarRegression:
    def __init__(self,X,Y):
        self.X=X
        self.Y=Y
        self.m=0
        self.b=0
        self.n=len(X) 
        self.alpha=0.001
        self.nrEpochs=10000
        self.epsilon=0.1
        
    def mse(self):
        return np.sum((self.m*self.X+self.b-self.Y)**2)/self.n

    def update(self):
        self.m -= self.alpha * self.derivM()
        self.b -= self.alpha * self.derivB()

    def derivM(self):
        return np.sum((self.m*self.X+self.b-self.Y)*self.X)*2/self.n;

    def derivB(self):
        return np.sum(self.m*self.X+self.b-self.Y)*2/self.n

    def train(self):
        for i in range(self.nrEpochs):
            err = self.mse()
            if i%(self.nrEpochs/10)==0:
                print("epoch=",i,"error",err)
            if err < self.epsilon:
                break
            self.update()

    def predict(self,x):
        return self.m*x+self.b

    def test(self):
        for i in range(0,10):
            print(f"predict({i}) ",self.predict(i))

    def plot(self):
        plt.scatter(self.X,self.Y,color = "r", marker = "x", s = 20)
        plt.plot(self.X,self.predict(self.X),color = "g")
        plt.xlabel('x')
        plt.ylabel('y')
        plt.show()

X = [0,1,2,3,4,5,6,7,8,9]
Y = [1,3,2,5,7,8,8,9,10,12]
X = np.array(X)
Y = np.array(Y)

liniar = LiniarRegression(X,Y)
liniar.train()
liniar.test()
liniar.plot()

'''
X = [0,1,2,3,4,5,6,7,8,9]
Y = [1,3,2,5,7,8,8,9,10,12]

n = len(X)

# y = m*x+b
m = 0.2
b = 0.6

alpha = 0.0001
nrEpochs = 10000
epsilon = 0.1

def mse():
    error=0
    for x,y in zip(X,Y):
        error+=(m*x+b-y)**2
    return error/n

def update():
    global m
    global b
    m -= alpha * derivM()
    b -= alpha * derivB()

def derivM():
    s = 0
    for x,y in zip(X,Y):
        s += (m*x+b-y) * x  
    return s*2/n

def derivB():
    s = 0
    for x,y in zip(X,Y):
        s += (m*x+b-y) 
    return s*2/n

def train():
    for i in range(nrEpochs):
        err = mse()
        if i%(nrEpochs/10)==0:
            print("epoch=",i,"error",err)
        if err < epsilon:
            break
        update()

def predict(x):
    return m*x+b

def test():
    print("m=",m," b=",b)
    for i in range(0,10):
        print(f"predict({i}) ",predict(i))

def plot():
    plt.scatter(X,Y,color = "m", marker = "o", s = 30)
    y_pred = []
    for x in X:
        y_pred += [b+m*x]
    print(y_pred)
    
    plt.plot(np.array(X),np.array(y_pred),color = "g")

    plt.xlabel('x')
    plt.ylabel('y')
    plt.show()

train()
test()
print(X)
print(Y)
plot()
'''
