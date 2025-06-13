"""
    Train a model able to classify text into different categories.
"""

from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.naive_bayes import MultinomialNB
from sklearn.pipeline import make_pipeline


#get the data
#we will use a subset of categories

categories= ['talk.religion.misc', 'sci.space', 'comp.graphics']

#load the dataset
train= fetch_20newsgroups(subset='train', categories=categories)
test= fetch_20newsgroups(subset='test', categories=categories)

#make a pipeline
model= make_pipeline(TfidfVectorizer(), MultinomialNB())

#train the model and test it
model.fit(train.data, train.target)
predicted= model.predict(test.data)
#check the accuracy
from sklearn.metrics import accuracy_score
accuracy= accuracy_score(test.target, predicted)
print(f'Accuracy: {accuracy:.2f}')
#check the predictions
predicted_categories= model.predict(test.data)
for i in range(10):
    print(f'Predicted: {train.target_names[predicted_categories[i]]}, Actual: {train.target_names[test.target[i]]}')

#use seaborn to plot the confusion matrix
import seaborn as sns
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
conf_matrix = confusion_matrix(test.target, predicted)
sns.heatmap(conf_matrix.T,fmt="d",square=True, cbar=False,annot=True,xticklabels=train.target_names, yticklabels=train.target_names)
plt.xlabel('True label')
plt.ylabel('Predicted label')
plt.title('Confusion Matrix')
plt.show()

