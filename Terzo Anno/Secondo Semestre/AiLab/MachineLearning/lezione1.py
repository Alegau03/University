import seaborn as sburra
import matplotlib.pyplot as plt


#load the iris dataset
# [n_samples, n_features]
# the rows are the samples, the columns are the features
iris= sburra.load_dataset("iris")

sburra.pairplot(iris, hue="species", palette="husl")
plt.show()