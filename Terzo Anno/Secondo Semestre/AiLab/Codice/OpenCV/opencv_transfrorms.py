import cv2
import numpy as np

img = cv2.imread("Data/01-Data/lena.png")

"""
Traslazione dell'immagine
M= np.float32([[1,0,200],[0,1,50]])
h,w = img.shape[:2]
new_img= cv2.warpAffine(img,M,(1200,1200))



#Rotazione dell'immagine
    alfa beta (1-alfa)*center.x-beta*center.y
    -beta alfa beta*center.x+(1-alfa)*center.y
    
    alfa = scale * cosAngle
    beta = scale* sinAngle   


h,w = img.shape[:2] 
center = (w//2,h//2)
M= cv2.getRotationMatrix2D(center,180,1)
new_img= cv2.warpAffine(img,M,(h,w))
cv2.imshow("Rotated",new_img)
cv2.waitKey(0)
"""

#create 2 sets of points
pts_1= np.float32([
    [135,45],
    [385,45],
    [135,230],
])

pts_2 = np.float32([
    [135,45],
    [385,45],
    [150,230],
])

#get the matrix M that transform pts_1 into pts_2 (ci dice quale matrice utilizzare per passare dai punti1 ai punti2)
M= cv2.getAffineTransform(pts_1,pts_2)
new_img=cv2.warpAffine(pts_1,M,(1024,1024))
