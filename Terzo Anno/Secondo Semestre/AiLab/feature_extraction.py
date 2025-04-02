import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.imread('Data/01-Data/lena.png')

# create the feature extractor 

# SIFT 
sift = cv2.SIFT_create()

#compute the descriptors and the keypoints array

keypoints, descriptors = sift.detectAndCompute(img, None)
# draw the keypoints
img_sift = cv2.drawKeypoints(img, keypoints, img, (255,0,0), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
# show the image
cv2.imshow('SIFT Keypoints', img_sift)
cv2.waitKey(0)