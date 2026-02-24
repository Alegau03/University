import cv2
import numpy as np


my_kernel= np.array([
    
    [0,-1,0],
    [-1,5,-1],
    [0,-1,0]
])

img = cv2.imread("Data/01-Data/lena.png")
gray= cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
gray_y= cv2.Sobel(gray,-1,0-1)
gray_x= cv2.Sobel(gray,-1,1,0)

abs_x = cv2.convertScaleAbs(gray_x)
abs_y = cv2.convertScaleAbs(gray_y)

#filtered_img= cv2.filter2D(img,-1,my_kernel)
