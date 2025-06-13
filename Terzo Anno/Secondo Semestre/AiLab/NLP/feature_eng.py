#categorical data
import pandas as pd
from sklearn.feature_extraction import DictVectorizer
#Modo peggiore per trainare un modello, stiamo passando dati non omogenei
data= [
    {'price':200000, 'rooms':4, 'neighboorhood': 'Appia'},
    {'price':100000, 'rooms':2, 'neighboorhood': 'Tuscolana'},
    {'price':350000, 'rooms':3, 'neighboorhood': 'Colosseo'},
    {'price':120000, 'rooms':2, 'neighboorhood': 'Casilina'}
]

neigh= {'Appia':1, 'Tuscolana':2, 'Colosseo':3, 'Casilina':4} #il modello qui potrebbe pensare che Appia= colosseo - tuscolana che non ha senso!

#In questo caso il modello non ha modo di capire che i dati sono categoriali e non numerici


#one hot encoding
vec= DictVectorizer(sparse=False, dtype=int) #sparse=False per avere un array numpy e non una matrice sparsa

res= vec.fit_transform(data) #fit_transform serve per fare il fit e la trasformazione in un colpo solo   
print(vec.get_feature_names_out()) #array numpy