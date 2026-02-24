#Versione migliore (scelgo i punti a runtime con il mouse)
import cv2
import numpy as np

#definisco la funzione di callback
def onClick(event,x,y,flags,params):
    #prendo le coordinate del click
    if event == cv2.EVENT_LBUTTONDOWN:
        #controllo se sono già a 4 punti
        if len(scr_points)<4:
            scr_points.append((x,y))
            #disegno un cerchio nel punto
            cv2.circle(img_copy,(x,y),15,(0,255,0),-1)
            cv2.imshow("Img",img_copy)

img=cv2.imread("Data/01-Data/gerry.png")

#copio l'immagine per pizzare un punto nel punto dove clicco sull'immagine, li inserisco nella copia così che non saranno poi nell'immagine finale
img_copy = img.copy()


#creo il source point, inizialmente vuoto perchè lo riempirò con i punti che clicco
scr_points=[]
dst_points= np.float32([
    [0,0],
    [0,600],
    [800,600],
    [800,0]
])

#creo la finestra che prenderà i click
cv2.namedWindow("Img",cv2.WINDOW_KEEPRATIO)
cv2.imshow("Img",img_copy)
cv2.setMouseCallback("Img", onClick)
cv2.waitKey(0)

#get the homography matrix
scr2= np.float32(scr_points)
H = cv2.getPerspectiveTransform(scr2,dst_points)
#apply the homography matrix
new_img=cv2.warpPerspective(img,H,(600,800))
cv2.namedWindow("Result",cv2.WINDOW_KEEPRATIO)
cv2.imshow("Result",new_img)
cv2.waitKey(0)