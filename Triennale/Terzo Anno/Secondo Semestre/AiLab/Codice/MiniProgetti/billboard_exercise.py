import numpy as np
import cv2

# define the onclick function


def onClick(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        if len(dst_points) < 4:
            dst_points.append([x, y])
            cv2.circle(img_copy, (x, y), 15, (0, 0, 255), -1)
            cv2.imshow('Click', img_copy)


img = cv2.imread('./Data/01-Data/billboard.jpg')
img_copy = img.copy()
img2 = cv2.imread('./Data/01-Data/ezio.jpg')
# store the imgs data
img_h, img_w = img.shape[:2]
img2_h, img2_w = img2.shape[:2]
# create source and destination points
src_points = np.float32([
    [0, 0],
    [0, img2_h],
    [img2_w, img2_h],
    [img2_w, 0]
])
dst_points = []

# define a window allowing us to pick points
cv2.namedWindow('Click', cv2.WINDOW_KEEPRATIO)
cv2.setMouseCallback('Click', onClick)
cv2.namedWindow('Billboard', cv2.WINDOW_KEEPRATIO)
#cv2.imshow('Billboard', img)
#cv2.imshow('Ezio', img2)
#cv2.waitKey(0)


cv2.imshow('Click', img)
cv2.waitKey(0)
# compute the tranformation matrix
dst_float = np.float32(dst_points)
H = cv2.getPerspectiveTransform(src_points, dst_float)

# apply the transformation
warped = cv2.warpPerspective(img2, H, (img_w, img_h))
cv2.namedWindow('Warped', cv2.WINDOW_KEEPRATIO)
cv2.imshow('Warped', warped)
cv2.waitKey(0)


# Create a mask dropping the black pixels from the warped img
mask= cv2.bitwise_not(warped)
masked_bill= cv2.bitwise_and(masked_bill, mask)
#add the warped image to the billboard
masked_bill = cv2.bitwise_or(img, warped)
final= cv2.bitwise_or(masked_bill, warped)

cv2.namedWindow('warped',cv2.WINDOW_KEEPRATIO)
cv2.imshow('warped', final)
cv2.waitKey(0)
