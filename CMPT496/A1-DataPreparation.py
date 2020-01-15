
# coding: utf-8

# In[108]:


import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import numpy.random as nr
import math
from sklearn.tree import DecisionTreeClassifier
import sklearn.model_selection as ms
from sklearn import preprocessing

get_ipython().run_line_magic('matplotlib', 'inline')


# Load and join datasets. 

# In[109]:


bikeStats = pd.read_csv('AdvWorksCusts.csv')
bikeBuyer = pd.read_csv('AW_BikeBuyer.csv')
bikes = pd.merge(bikeStats, bikeBuyer, on='CustomerID')

print(bikes.shape)
print(bikes.CustomerID.unique().shape)
print(bikes['BikeBuyer'].value_counts())
bikes.head()


# In[110]:


bikes.dtypes


# In[111]:


#find unique item values with counts
for col in bikes.columns:
    print(col, "\n", bikes[col].value_counts(), "\n\n")
    


# In[112]:


#search for empty cells
(bikes.astype(np.object) == '').any()


# Feature Engeneering
# Transforms BirthDate into Age in 2019.
# Takes the AreaCode from the PhoneNumber.

# In[113]:


Age = [2019 - int(x[0:4]) for x in bikes.BirthDate]
bikes['Age'] = Age

AreaCode = [int(x[7:10]) if '(' in x else int(x[4:7]) for x in bikes.PhoneNumber]
bikes['AreaCode'] = AreaCode


# Remove duplicate rows.

# In[114]:


bikes.drop_duplicates(subset = 'CustomerID', keep = 'first', inplace = True)
print(bikes.shape)
print(bikes.CustomerID.unique().shape)
print(bikes['BikeBuyer'].value_counts())


# In[115]:


labels = ['City', 'StateProvinceName', 'CountryRegionName', 'PostalCode', 'Education', 'Occupation', 'Gender', 'MaritalStatus', 'HomeOwnerFlag', 'NumberCarsOwned', 
                            'NumberChildrenAtHome', 'TotalChildren', 'YearlyIncome', 'Age', 'AreaCode', 'BikeBuyer']
for col in bikes.columns:
    if col not in labels:
        bikes.drop(col, axis=1, inplace=True)
features = pd.get_dummies(bikes)
print(features.shape)
features.dtypes


# In[116]:


features.to_csv('BikesProcessed.csv')
features.head()


# Features that are not included in the dataset:
#     'BirthDate', 'CustomerID', 'Title', 'FirstName', 'MiddleName', 'LastName', 'Suffix', 'AddressLine1', 'AddressLine2', 'PhoneNumber'
#     
# Code below is based on:
# https://www.edx.org/course/principles-of-machine-learning-python-edition

# def encode_string(cat_features):
#     ## First encode the strings to numeric categories
#     enc = preprocessing.LabelEncoder()
#     enc.fit(cat_features)
#     enc_cat_features = enc.transform(cat_features)
#     ## Now, apply one hot encoding
#     ohe = preprocessing.OneHotEncoder()
#     encoded = ohe.fit(enc_cat_features.reshape(-1,1))
#     return encoded.transform(enc_cat_features.reshape(-1,1)).toarray()
# 
# categorical_columns = ['StateProvinceName', 'CountryRegionName', 'PostalCode', 'Education', 'Occupation', 
#                        'Gender', 'MaritalStatus']
# 
# features = encode_string(bikes['City'])
# for col in categorical_columns:
#     temp = encode_string(bikes[col])
#     features = np.concatenate([features, temp], axis = 1)
#     
# features = np.concatenate([features, np.array(bikes[['HomeOwnerFlag', 'NumberCarsOwned', 
#                             'NumberChildrenAtHome', 'TotalChildren', 'YearlyIncome', 'Age', 'AreaCode', 'BikeBuyer']])], axis = 1)
#     
# print(features.shape)
# print(features[:2, :]) 

# Save processed data as a new dataset.

# processedData = pd.DataFrame(features)
# processedData.to_csv('BikesProcessed.csv')
# processedData.head()
