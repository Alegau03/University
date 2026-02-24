import cv2
import matplotlib.pyplot as plt
img = cv2.imread('./Data/01-Data/lena.png')


"""
#parte 1
#hist= cv2.calcHist([img],[0],None,[256],[0,256]) da usare per immagini in bianco e nero

color= ('b', 'g', 'r')
for i,col in enumerate(color):
    hist= cv2.calcHist([img],[i],None,[256],[0,256])
    plt.plot(hist, color=col)
    plt.xlim([0,256])


plt.title('Histogram')
plt.xlabel('Pixel value')
plt.ylabel('Frequency')
plt.show()"""

#Parte 2 , cambiando color space

hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

#get the channels of the images
h,s,v= cv2.split(hsv_img)
#equalize the histogram of the V channel
eq_v= cv2.equalizeHist(v)

img_eq= cv2.merge((h,s,eq_v))
hsv_img_eq= cv2.cvtColor(img_eq, cv2.COLOR_HSV2BGR)
cv2.imshow('Image', img)
cv2.imshow('Equalized image', hsv_img_eq)
cv2.waitKey(0)