import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.imread('Data/01-Data/tsukuba.png', 0)

eq_img= cv2.equalizeHist(img)

#Useremo un algoritmo chiamato clahe, server per migliorare le immagini in modo locale in particolare usa una griglia di pixel 

clahe= cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
clahe_img= clahe.apply(img)
eq_img= cv2.equalizeHist(img)

shoe_img = np.hstack((img, eq_img, clahe_img))
cv2.imshow('img', shoe_img)
cv2.waitKey(0)