"program that uses unsupervised learning "

import numpy as np
import cv2
from sklearn.cluster import KMeans


#load the image
img_arr = cv2.imread("Data/01-Data/lena.png")

#prepare the image for sklearn
(h,w,c)= img_arr.shape
img2D= img_arr.reshape(h*w,c)


kmeans_model=KMeans(n_clusters=8)
cluster_label= kmeans_model.fit_predict(img2D)

#convert the centroids coordinates to int
rgb_cols= kmeans_model.cluster_centers_.round(0).astype(int)

#reconstruct the origial image

img_quant = np.reshape(rgb_cols[cluster_label], (h,w,c))
img_quant = img_quant.astype(np.uint8)



cv2.imshow("Quantized Image", img_quant)
cv2.waitKey(0)