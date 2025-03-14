import cv2
import numpy as np

img = cv2.imread("Data/01-Data/lena.png")
# Resize the image
resized=    cv2.resize(img,(100,100),interpolation=cv2.INTER_AREA)
#cv2.imshow("Resized",resized)   
#cv2.waitKey(0)



#define the transaltion matrix
M= np.float32([[1,0,200],[0,1,50]])
#apply the translation
h,w= img.shape[:2]
translated= cv2.warpAffine(img,M,(w,h))
cv2.imshow("Translated",translated)
cv2.waitKey(0)