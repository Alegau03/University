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
#cv2.imshow('SIFT Keypoints', img_sift)
#cv2.waitKey(0)


# now we use the SURF algorithm for feature extraction
#f_ex= cv2.xfeatures2d.SURF_create() #non utilizzabile perchè non opensource

# Utilizziamo ora ORB
orb = cv2.ORB_create()
# compute the descriptors and the keypoints array
keypoints, descriptors = orb.detectAndCompute(img, None)
# draw the keypoints
img_orb = cv2.drawKeypoints(img, keypoints, img, (255,0,0), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
# show the image
#cv2.imshow('ORB Keypoints', img_orb)
#cv2.waitKey(0)


# Ora usiamo AKAZA

akaze = cv2.AKAZE_create()
# compute the descriptors and the keypoints array
keypoints, descriptors = akaze.detectAndCompute(img, None)
# draw the keypoints
img_akaze = cv2.drawKeypoints(img, keypoints, img, (255,0,0), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
# show the image
cv2.imshow('AKAZE Keypoints', img_akaze)
cv2.waitKey(0)