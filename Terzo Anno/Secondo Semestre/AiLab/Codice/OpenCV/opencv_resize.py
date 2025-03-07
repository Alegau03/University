import cv2
import numpy as np

img = cv2.imread("Data/01-Data/lena.png")
# Resize the image
resized=    cv2.resize(img,(100,100),interpolation=cv2.INTER_AREA)
cv2.imshow("Resized",resized)   
cv2.waitKey(0)