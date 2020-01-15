
# coding: utf-8

# In[55]:


import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import numpy.random as nr
import math
from sklearn.tree import DecisionTreeClassifier
import sklearn.model_selection as ms
from sklearn import preprocessing
import sklearn.decomposition as skde
import sklearn.metrics as sklm
import sklearn.datasets as skds

get_ipython().run_line_magic('matplotlib', 'inline')


# Loads the dataset.

# In[56]:


bikesData = pd.read_csv('BikesProcessed.csv')
Labels = bikesData['BikeBuyer']
bikesData.drop(bikesData.columns[0], axis=1, inplace=True)
bikesData.drop(['BikeBuyer'], axis=1, inplace=True)
Features = np.array(bikesData)
print(Features.shape)
bikesData.head()


# Trains the dataset.
# Code below based on:
# https://www.edx.org/course/principles-of-machine-learning-python-edition

# In[57]:


# Randomly sample cases to create independent training and test data

nr.seed(1115)
indx = range(Features.shape[0])
indx = ms.train_test_split(indx, test_size = 5000)
X_train = Features[indx[0],:]
y_train = np.ravel(Labels[indx[0]])
X_test = Features[indx[1],:]
y_test = np.ravel(Labels[indx[1]])


# Scales the dataset
# Code below based on:
# https://www.edx.org/course/principles-of-machine-learning-python-edition

# In[58]:


#Rescale numeric features
scaler = preprocessing.StandardScaler().fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)
print(X_train.shape)
X_train[:5,:]


# #X_train, X_test, y_train, y_test = ms.train_test_split(
# #    bikeStats, bikeBuyer, stratify=bikeBuyer, random_state=42)
# print("Depth\t Training Set %\t Test Set %")
# for i in range(1,31):
#     tree = DecisionTreeClassifier(max_depth = i, random_state=0)
#     tree.fit(X_train, y_train)
#     print(i, "\t %f" % tree.score(X_train, y_train), "\t %f" % tree.score(X_test, y_test))
#     #print("max depth of tree:", i)
#     #print("accuracy on training set: %f" % tree.score(X_train, y_train))
#     #print("accuracy on test set: %f" % tree.score(X_test, y_test))
#     #print("\n")

# Finds the most important features
# Code based on:
# https://scikit-learn.org/stable/auto_examples/ensemble/plot_forest_importances.html

# In[59]:


# Build a tree and compute the feature importances
tree = DecisionTreeClassifier(random_state=0)

tree.fit(X_train, y_train)
importances = tree.feature_importances_

indices = np.argsort(importances)[::-1]

# Print the feature ranking
print("Feature ranking:")

for f in range(X_train.shape[1]):
    print("%d. feature %s (%f)" % (f + 1, bikesData.columns[int(indices[f])], importances[indices[f]]))


# Code below based on:
# Introduction to Machine Learning with Python (2016).

# In[60]:


# Creates a picture as a .dot file with max_depth = 5
from sklearn.tree import export_graphviz
export_graphviz(tree, out_file="mytree.dot", max_depth = 5,
                feature_names=indices, impurity=False, filled=True)
    
import graphviz
with open("mytree.dot") as f:
    dot_graph = f.read()
graphviz.Source(dot_graph)
f.close()


# In[61]:


importances.sort()
importances = importances[::-1]
important_features = [bikesData.columns[int(indices[x])] for x in range(5)]
plt.bar(range(5), importances[:5], tick_label = important_features[:5])
plt.xticks(rotation=90)
plt.ylabel('Feature importance')


# PCA
# Code below based on:
# https://www.edx.org/course/principles-of-machine-learning-python-edition

# In[62]:


pca_mod = skde.PCA()
pca_comps = pca_mod.fit(X_train)
pca_comps


# In[63]:


print(pca_comps.explained_variance_ratio_)
print(np.sum(pca_comps.explained_variance_ratio_))


# In[64]:


def plot_explained(mod):
    comps = mod.explained_variance_ratio_
    x = range(len(comps))
    x = [y + 1 for y in x]          
    plt.plot(x,comps)

plot_explained(pca_comps)


# In[65]:


importance = pca_comps.explained_variance_ratio_
importance.sort()
importance = importance[::-1]
plt.bar(range(10), importance[:10])#, tick_label = indices[:5]
plt.xticks(rotation=90)
plt.ylabel('Explained Variance Ratio')


# pca_mod_5 = skde.PCA(n_components = 7)
# pca_mod_5.fit(X_train)
# Comps = pca_mod_5.transform(X_train)
# Comps.shape

# nr.seed(1115)
# pca_mod = DecisionTreeClassifier() 
# pca_mod.fit(Comps, y_train)

# def score_model(probs, threshold):
#     return np.array([1 if x > threshold else 0 for x in probs[:,1]])
# 
# def print_metrics(labels, probs, threshold):
#     scores = score_model(probs, threshold)
#     metrics = sklm.precision_recall_fscore_support(labels, scores)
#     conf = sklm.confusion_matrix(labels, scores)
#     print('                 Confusion matrix')
#     print('                 Score positive    Score negative')
#     print('Actual positive    %6d' % conf[0,0] + '             %5d' % conf[0,1])
#     print('Actual negative    %6d' % conf[1,0] + '             %5d' % conf[1,1])
#     print('')
#     print('Accuracy        %0.2f' % sklm.accuracy_score(labels, scores))
#     print('AUC             %0.2f' % sklm.roc_auc_score(labels, probs[:,1]))
#     print('Macro precision %0.2f' % float((float(metrics[0][0]) + float(metrics[0][1]))/2.0))
#     print('Macro recall    %0.2f' % float((float(metrics[1][0]) + float(metrics[1][1]))/2.0))
#     print(' ')
#     print('           Positive      Negative')
#     print('Num case   %6d' % metrics[3][0] + '        %6d' % metrics[3][1])
#     print('Precision  %6.2f' % metrics[0][0] + '        %6.2f' % metrics[0][1])
#     print('Recall     %6.2f' % metrics[1][0] + '        %6.2f' % metrics[1][1])
#     print('F1         %6.2f' % metrics[2][0] + '        %6.2f' % metrics[2][1])
#     
# def plot_auc(labels, probs):
#     ## Compute the false positive rate, true positive rate
#     ## and threshold along with the AUC
#     fpr, tpr, threshold = sklm.roc_curve(labels, probs[:,1])
#     auc = sklm.auc(fpr, tpr)
#     
#     ## Plot the result
#     plt.title('Receiver Operating Characteristic')
#     plt.plot(fpr, tpr, color = 'orange', label = 'AUC = %0.2f' % auc)
#     plt.legend(loc = 'lower right')
#     plt.plot([0, 1], [0, 1],'r--')
#     plt.xlim([0, 1])
#     plt.ylim([0, 1])
#     plt.ylabel('True Positive Rate')
#     plt.xlabel('False Positive Rate')
#     plt.show()
#     
# probabilities = pca_mod.predict_proba(pca_mod.feature_importances_.reshape(-1, 1))
# print_metrics(y_test, probabilities, 0.3)    
# plot_auc(y_test, probabilities)

# Nested cross validation
# Code below based on:
# https://www.edx.org/course/principles-of-machine-learning-python-edition

# In[66]:


nr.seed(123)
inside = ms.KFold(n_splits=2, shuffle = True)
nr.seed(321)
outside = ms.KFold(n_splits=2, shuffle = True)


# In[67]:


## Define the dictionary for the grid search and the model object to search on
param_grid = {"max_features": [1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50], "max_depth":[1, 5, 10, 15, 20, 25, 30, 35, 40]}
## Define the random forest model
nr.seed(3456)
rf_clf = DecisionTreeClassifier()

## Perform the grid search over the parameters
nr.seed(4455)
rf_clf = ms.GridSearchCV(estimator = rf_clf, param_grid = param_grid, 
                      cv = inside, # Use the inside folds
                      scoring = 'roc_auc',
                      return_train_score = True)
rf_clf.fit(Features, Labels)
print(rf_clf.best_estimator_.max_features)
print(rf_clf.best_estimator_.max_depth)


# In[68]:


nr.seed(498)
cv_estimate = ms.cross_val_score(rf_clf, Features, Labels, 
                                 cv = outside) # Use the outside folds

print('Mean performance metric = %4.3f' % np.mean(cv_estimate))
print('SDT of the metric       = %4.3f' % np.std(cv_estimate))
print('Outcomes by cv fold')
for i, x in enumerate(cv_estimate):
    print('Fold %2d    %4.3f' % (i+1, x))


# In[69]:


# Randomly sample cases to create independent training and test data

nr.seed(1115)
indx = range(Features.shape[0])
indx = ms.train_test_split(indx, test_size = 5000)
X_train = Features[indx[0],:]
y_train = np.ravel(Labels[indx[0]])
X_test = Features[indx[1],:]
y_test = np.ravel(Labels[indx[1]])


# In[70]:


nr.seed(1115)
rf_mod = DecisionTreeClassifier(max_features = rf_clf.best_estimator_.max_features, 
                                min_samples_leaf = rf_clf.best_estimator_.max_depth) 
rf_mod.fit(X_train, y_train)


# In[71]:


def score_model(probs, threshold):
    return np.array([1 if x > threshold else 0 for x in probs[:,1]])

def print_metrics(labels, probs, threshold):
    scores = score_model(probs, threshold)
    metrics = sklm.precision_recall_fscore_support(labels, scores)
    conf = sklm.confusion_matrix(labels, scores)
    print('                 Confusion matrix')
    print('                 Score positive    Score negative')
    print('Actual positive    %6d' % conf[0,0] + '             %5d' % conf[0,1])
    print('Actual negative    %6d' % conf[1,0] + '             %5d' % conf[1,1])
    print('')
    print('Accuracy        %0.2f' % sklm.accuracy_score(labels, scores))
    print('AUC             %0.2f' % sklm.roc_auc_score(labels, probs[:,1]))
    print('Macro precision %0.2f' % float((float(metrics[0][0]) + float(metrics[0][1]))/2.0))
    print('Macro recall    %0.2f' % float((float(metrics[1][0]) + float(metrics[1][1]))/2.0))
    print(' ')
    print('           Positive      Negative')
    print('Num case   %6d' % metrics[3][0] + '        %6d' % metrics[3][1])
    print('Precision  %6.2f' % metrics[0][0] + '        %6.2f' % metrics[0][1])
    print('Recall     %6.2f' % metrics[1][0] + '        %6.2f' % metrics[1][1])
    print('F1         %6.2f' % metrics[2][0] + '        %6.2f' % metrics[2][1])
    
def plot_auc(labels, probs):
    ## Compute the false positive rate, true positive rate
    ## and threshold along with the AUC
    fpr, tpr, threshold = sklm.roc_curve(labels, probs[:,1])
    auc = sklm.auc(fpr, tpr)
    
    ## Plot the result
    plt.title('Receiver Operating Characteristic')
    plt.plot(fpr, tpr, color = 'orange', label = 'AUC = %0.2f' % auc)
    plt.legend(loc = 'lower right')
    plt.plot([0, 1], [0, 1],'r--')
    plt.xlim([0, 1])
    plt.ylim([0, 1])
    plt.ylabel('True Positive Rate')
    plt.xlabel('False Positive Rate')
    plt.show()

probabilities = rf_mod.predict_proba(X_test)
print_metrics(y_test, probabilities, 0.5)
plot_auc(y_test, probabilities)     


# Get the most important features

# In[72]:


importance = rf_mod.feature_importances_
importance.sort()
importance = importance[::-1]
plt.bar(range(4), importance[:4]) #, tick_label = ['666', '668', '669', '662']
plt.xticks(rotation=90)
plt.ylabel('Feature importance')

