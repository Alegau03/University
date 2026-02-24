
# ADDESTRIAMO UN MODELLO PER LA CLASSIFICAZIONE! LEZIONE 2.1

import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
#prepariamo il dataset per la CLASSIFICAZIONE
iris= sns.load_dataset("iris")

x_iris= iris.drop(columns=["species"])

y= iris["species"]
#creiamo il dataset per il training e il test
x_train, x_test, y_train, y_test= train_test_split(x_iris, y, test_size=0.5, random_state=42)

#creiamo un instanza del modello
model= GaussianNB()

#alleniamo il modello
model.fit(x_train, y_train)
#prediciamo il risultato
predictions= model.predict(x_test)

#calcoliamo l'accuratezza
accuracy= accuracy_score(y_test, predictions)
print(f"Accuratezza del modello: {accuracy:.2f}")

