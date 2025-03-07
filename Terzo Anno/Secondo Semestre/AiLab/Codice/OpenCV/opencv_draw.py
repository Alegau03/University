import cv2
import numpy as np

#create canvas

canvas= np.zeros((1000,1000,3),dtype="uint8")
aereo = cv2.imread("Data/01-Data/small-airplane-emoji-clipart-xl.png")
if aereo is None:
	raise FileNotFoundError("Image file not found. Please check the path.")
# define the green color
green=(0,255,0)
red=(0,0,255)
yellow=(0,255,255)
#driw the line
cv2.line(canvas,(100,20),(100,200),green,3)
cv2.line(canvas,(100,20),(130,20),green,3)
cv2.line(canvas,(130,20),(130,200),green,3)
cv2.line(canvas,(115,20),(115,30),green)
#draw pisello
cv2.circle(canvas,(100,200),15,red,-1)
cv2.circle(canvas,(130,200),15,yellow,-1)
#scrivere nell'immagine
cv2.putText(canvas,"Forza",(100,250),cv2.FONT_HERSHEY_SIMPLEX,1,red,2)
cv2.putText(canvas,"Roma",(200,250),cv2.FONT_HERSHEY_SIMPLEX,1,yellow,2)
cv2.rectangle(canvas,(500,300),(550,450),green,3)
cv2.rectangle(canvas,(600,300),(650,450),green,3)
x_offset=200
y_offset=300
#Put the image on the canvas
canvas[y_offset:y_offset+aereo.shape[0],x_offset:x_offset+aereo.shape[1]]=aereo



img= cv2.imread("Data/01-Data/lena.png")
b,g,r=cv2.split(img)
# b=img[:,:,0] numpy version
#channels= np.hstack([b,g,r])
b= b*b
r= g*b*r
g= g+123
# MERGE CHANNELS TOGHETER
new= cv2.merge([r,b,g])

cv2.imshow("New",new)
cv2.waitKey(0)
