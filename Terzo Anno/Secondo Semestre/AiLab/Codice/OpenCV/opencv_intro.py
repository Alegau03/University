import cv2
import numpy as np

#read the image
img = cv2.imread('Data/01-Data/lena.png')

# access image properties

print(f'The width is {img.shape[1]} pixels')
print(f'The height is {img.shape[0]} pixels')
print(f'The number of channels is {img.shape[2]}')

#show image on the screen

#cv2.imshow('foto', img)
#cv2.waitKey(0)

# save on pc
#cv2.imwrite('img.png', img)


#create a black image
#empty= np.zeros((200,200), dtype=np.uint8)
#cv2.imwrite('empty.png', empty)


img[:150,:150] = [255,255,255]
cv2.imshow('foto', img)
cv2.waitKey(0)