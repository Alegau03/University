import numpy as np
import cv2

# load the image
img = cv2.imread('./01-Data/lena.png')

# create a binary mask
mask = np.zeros(img.shape, dtype=np.uint8)
mask = cv2.rectangle(mask, (100, 100), (250, 250), (255, 255, 255), -1)
result = cv2.bitwise_and(img, mask)
cv2.imshow('Mask', result)
cv2.waitKey(0)