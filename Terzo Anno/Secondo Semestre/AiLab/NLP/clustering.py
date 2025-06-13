from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns


iris = sns.load_dataset("iris")
X_iris= iris.drop('species', axis=1)
y_iris= iris["species"]

model= KMeans(n_clusters=3, random_state=42)
model.fit(X_iris)
y_pred= model.predict(X_iris)
plt.scatter(X_iris['sepal_length'], X_iris['sepal_width'], c=y_pred)
plt.scatter(model.cluster_centers_[:, 0], model.cluster_centers_[:, 1], s=300, c='red', label='Centroidi')
plt.show()