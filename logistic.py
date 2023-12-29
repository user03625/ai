import pandas as pd

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import math
import numpy as np

class LogisticRegression:
    def __init__(self):
        self.w = None 
        self.alpha = 0.01
        self.nrEpochs = 100
        self.bias = 0
        self.nrFeatures = 0
        self.nrSamples = 0

    def fit(self,x,y):
        self.nrSamples, self.nrFeatures = x.shape
        self.w = np.zeros(self.nrFeatures)
        for epoch in range(self.nrEpochs):
            predictions = self.sigmoid(np.dot(x,self.w)+self.bias)
            dw = np.dot(x.T,predictions-y)/self.nrSamples
            db = np.sum(predictions-y)/self.nrSamples
            
            self.w -= self.alpha*dw
            self.bias -= self.alpha*db 
            
    def sigmoid(self,z):
        return 1/(1+np.exp(-z))
                

    def predict(self,x):
        y_pred = self.sigmoid(np.dot(x,self.w)+self.bias)
        return np.array([0 if y<0.5 else 1 for y in y_pred])

    def accuracy(self, y_pred, labels):
        s=np.sum(y_pred==labels)/len(y_pred)
        print("accuracy: ",s)


df = pd.read_csv('logistic/data.csv')
df_dummies = pd.get_dummies(data=df,columns=['Gender'])

X = df_dummies.drop('Purchased',axis=1)
Y = df_dummies['Purchased']

X_train, X_test, Y_train, Y_test = train_test_split(X,Y,test_size = 0.25, random_state = 0)

sc = StandardScaler()

X_train = sc.fit_transform(X_train)
X_train = np.array(X_train)
X_test = sc.transform(X_test)
X_test = np.array(X_test)


classifier = LogisticRegression()
classifier.fit(X_train,Y_train.array)
Y_pred = classifier.predict(X_test)
classifier.accuracy(Y_pred,Y_test.array)



