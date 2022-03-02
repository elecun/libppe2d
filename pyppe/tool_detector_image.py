
'''
Tool-tip detector from image
'''

import cv2
from cv2 import COLOR_GRAY2BGR
import numpy as np
from cv2 import COLOR_RGB2GRAY

if __name__ == "__main__":
    
    # 1. load source image
    raw = cv2.imread("./experiments/0216_tool_1/38.jpg")
    gray = cv2.cvtColor(raw, COLOR_RGB2GRAY)

    #camera calibration (for uc762c camera model)
    h,w = gray.shape[:2]
    mtx = np.matrix([[2517.792, 0., 814.045],[0., 2514.767, 567.330],[0., 0., 1.]])
    dist = np.matrix([[-0.361044, 0.154482, 0.000808, 0.000033, 0.]])
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
    gray = cv2.undistort(gray, mtx, dist, None, newcameramtx)
    
    # change raw
    out = cv2.cvtColor(gray, COLOR_GRAY2BGR)
    monitor = np.zeros((h,w,3), np.uint8)

    # draw wafer circle
    h,w = gray.shape[:2]
    cv2.circle(monitor, (int(w/2), int(h/2)), 300, (0,255,0), 1)
    

    #binarization
    #bin = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 15, 5)
    ret, bin = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    # contour
    contours, hierarchy = cv2.findContours(bin, cv2.RETR_LIST, cv2.CHAIN_APPROX_TC89_L1)
    for cnt in contours:
        if cv2.contourArea(cnt)>5000:
            cv2.drawContours(out, [cnt], 0, (0, 255, 0), 3)  # blue
    
    # for cnt in contours:
    #     epsilon = 0.02 * cv2.arcLength(cnt, True)
    #     approx = cv2.approxPolyDP(cnt, epsilon, True)
    #     print( len(approx))
    #     cv2.drawContours(raw,[approx],0,(0,255,255),5)
    

    # fast = cv2.FastFeatureDetector_create(3)
    # keypoints = fast.detect(gray, None)
    # img = cv2.drawKeypoints(raw, keypoints, None)
    

    cv2.imshow("result", out)
    cv2.imshow("monitor", monitor)
    while True:
        key = cv2.waitKey(0)
        if key == 27:
            cv2.destroyAllWindows()
            break