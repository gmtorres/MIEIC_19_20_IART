import sys

import numpy as np
import pandas as pd
import joblib


from sklearn.model_selection import train_test_split
from sklearn import preprocessing

from sklearn.preprocessing import OneHotEncoder
from sklearn.preprocessing import LabelEncoder
from sklearn.compose import ColumnTransformer 

from sklearn import tree
from sklearn.ensemble import RandomForestRegressor

from sklearn.pipeline import make_pipeline
from sklearn.model_selection import GridSearchCV


from sklearn.metrics import mean_squared_error, r2_score

class MultiColumnLabelEncoder:
    def __init__(self, columns = None):
        self.columns = columns # list of column to encode    
    def fit(self, X, y=None):
        return self   
    def transform(self, X):    
        output = X.copy()   
        if self.columns is not None:
            for col in self.columns:
                output[col] = LabelEncoder().fit_transform(output[col])
        else:
            for colname, col in output.iteritems():
                output[colname] = LabelEncoder().fit_transform(col)    
        return output 
    def fit_transform(self, X, y=None):
        return self.fit(X, y).transform(X)

    
    


dataset_url = 'http://mlr.cs.umass.edu/ml/machine-learning-databases/abalone/abalone.data'
data = pd.read_csv(dataset_url, sep=',' , 
                   names=['sex',
                          'length',
                          'diameter',
                          'height',
                          'whole_weight',
                          'shucked_weight',
                          'viscera_weight',
                          'shell_weight',
                          'rings'])
print(data.head())
print(data.shape)
print(data.describe())

y = data.rings
X = data.drop('rings', axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, 
                                                    test_size=0.2, 
                                                    random_state=123)
le = MultiColumnLabelEncoder(['sex'])
X_train_le = le.fit_transform(X_train)
#print(X_train_le)

ct = ColumnTransformer([("Sex", OneHotEncoder(),[0])], remainder="passthrough")
X_train_ohe = ct.fit_transform(X_train_le) 

#print(X_train_ohe)

X_test_le = le.transform(X_test)
X_test_ohe = ct.transform(X_test_le)


scaler = preprocessing.StandardScaler().fit(X_train_ohe)
X_train_scaled = scaler.transform(X_train_ohe)
X_test_scaled = scaler.transform(X_test_ohe)


clf = tree.DecisionTreeClassifier()

clf.fit(X_train_scaled,y_train)

y_pred = clf.predict(X_test_scaled)

print(r2_score(y_test, y_pred))

 
print(mean_squared_error(y_test, y_pred))

'''
pipeline = make_pipeline(preprocessing.StandardScaler(), 
                         tree.DecisionTreeClassifier())


clf = tree.DecisionTreeClassifier()

clf.fit(X_train,y_train)


y_pred = clf.predict(X_test)


print(r2_score(y_test, y_pred))

 
print(mean_squared_error(y_test, y_pred))


'''







