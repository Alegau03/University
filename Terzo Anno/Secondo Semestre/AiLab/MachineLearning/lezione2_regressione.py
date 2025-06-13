# ADDESTRIAMO UN MODELLO PER LA REGRESSIONE! LEZIONE 2.2

import matplotlib.pyplot as plt
import numpy as np
from sklearn.linear_model import LinearRegression

#generatore di numeri casuali
rng = np.random.RandomState(42)

#generiamo i dati
x= 10 * rng.rand(50)
y= 2 * x - 1 + rng.randn(50)

plt.scatter(x, y, color='blue', label='Dati')
#plt.show()

#creiamo un instanza del modello di regressione lineare

model= LinearRegression(fit_intercept=True)

# preripariamo i dati per il modello
x = x[:, np.newaxis] # aggiungiamo una dimensione
# addestriamo il modello
model.fit(x, y)
# stampiamo i coefficienti

# stampiamo la previsione
xtest = np.linspace(-1, 11)
Xtest= xtest[:, np.newaxis] # aggiungiamo una dimensione

# testiamo il modello
y_prediction = model.predict(Xtest)

plt.scatter(xtest, y_prediction, color='green', label='Dati')
plt.plot(Xtest, y_prediction, color='red', label='Modello di regressione')
plt.show()