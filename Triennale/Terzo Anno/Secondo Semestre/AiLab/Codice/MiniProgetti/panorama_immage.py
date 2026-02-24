

'''
Script che carica due immagini e automaticamente estrae le features e le unisce
'''

import cv2
import numpy as np
import matplotlib.pyplot as plt


#Load the images

img1= cv2.imread("Data/01-Data/right.png")
img2= cv2.imread("Data/01-Data/left.png")

#create the feature detector

orb= cv2.ORB_create()


#compute both the features 

kpts1,desc1 = orb.detectAndCompute(img1,None)
kpts2,desc2= orb.detectAndCompute(img2,None)


#create the matcher

matcher= cv2.BFMatcher(cv2.NORM_HAMMING)
matches= matcher.knnMatch(desc1,desc2,k=2)


#perform the ratio test

good_matches= []
for m,n in matches:
    if m.distance < 0.2*n.distance:
        good_matches.append(m)
        
#check if we have at least 4 points for computing
#the prerspective transformation matrix
#in OpenCV the two images involved in the match
# are called queryImg and TrainImg
# queryImg is the index of the point beloning to queryImg
# while trainIdx is the point belonging to trainImg
if len(good_matches)>=4:
        scr_points= np.float32([kpts1[m.queryIdx].pt for m in good_matches])
        dst_points= np.float32([kpts2[m.trainIdx].pt for m in good_matches])
        
        M,mask= cv2.findHomography(scr_points,dst_points)
        
        #transform the images to make them fit
        dst=cv2.warpPerspective(img1,M,(img1.shape[1]+img2.shape[1],img1.shape[0]))
        
        #copy the pixels of img1 inside dst that already contains img2
        dst[0:img2.shape[0],0:img2.shape[1]]= img2
        #show the result
        cv2.imshow("Panorama",dst)
        cv2.waitKey(0)