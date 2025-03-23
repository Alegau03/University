import cv2
import numpy as np


#load a color image

img = cv2.imread("Data/01-Data/lena.png")

#create a binary mask
mask=np.zeros(img.shape,dtype=np.uint8)
mask= cv2.rectangle(mask,(100,100),(200,200),(255,255,255),-1)

cv2.imshow("Mask",mask)
cv2.waitKey(0)