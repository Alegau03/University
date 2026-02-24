import cv2
import numpy as np


#Prima versione, do i punti a mano nel programma


img=cv2.imread("Data/01-Data/gerry.png")
 
 
#define the four corners of the original image, fatto facilemnte con le coordinate poi fatto graricamente
#tl - 28,227
#bl - 131,987
#bg - 730,860
#tr - 572,149

scr_points= np.float32([
    [28,227],
    [131,987],
    [730,860],
    [572,149]
]) 
#define the destinations points, the corners of the resolution of the final image

dst_points= np.float32([
    [0,0],
    [0,800],
    [600,800],
    [600,0]
])
#get the transformation matrix, USO GETPERSPECTIVE PERCHè HO 4 PUNTI
M=cv2.getPerspectiveTransform(scr_points,dst_points)
#get the transformed image
new_img=cv2.warpPerspective(img,M,(600,800)) # USO SEMPRE PERSPECTIVE PERCHè 4 PUNTI

cv2.namedWindow("Original",cv2.WINDOW_KEEPRATIO)
cv2.imshow("Img",new_img)
cv2.waitKey(0)
