
'''
Wafer Pose Estimation Test
'''

import cv2
from cv2 import COLOR_GRAY2BGR
import numpy as np
from cv2 import COLOR_RGB2GRAY

# ARUCO_DICT = {
# 	"DICT_4X4_50": cv2.aruco.DICT_4X4_50,
# 	"DICT_4X4_100": cv2.aruco.DICT_4X4_100,
# 	"DICT_4X4_250": cv2.aruco.DICT_4X4_250,
# 	"DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
# 	"DICT_5X5_50": cv2.aruco.DICT_5X5_50,
# 	"DICT_5X5_100": cv2.aruco.DICT_5X5_100,
# 	"DICT_5X5_250": cv2.aruco.DICT_5X5_250,
# 	"DICT_5X5_1000": cv2.aruco.DICT_5X5_1000,
# 	"DICT_6X6_50": cv2.aruco.DICT_6X6_50,
# 	"DICT_6X6_100": cv2.aruco.DICT_6X6_100,
# 	"DICT_6X6_250": cv2.aruco.DICT_6X6_250,
# 	"DICT_6X6_1000": cv2.aruco.DICT_6X6_1000,
# 	"DICT_7X7_50": cv2.aruco.DICT_7X7_50,
# 	"DICT_7X7_100": cv2.aruco.DICT_7X7_100,
# 	"DICT_7X7_250": cv2.aruco.DICT_7X7_250,
# 	"DICT_7X7_1000": cv2.aruco.DICT_7X7_1000,
# 	"DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL,
# }

        


if __name__ == "__main__":

    # use camera device
    device = cv2.VideoCapture(0)
    device.set(cv2.CAP_PROP_FRAME_WIDTH, 1600)
    device.set(cv2.CAP_PROP_FRAME_HEIGHT, 1200)
    device.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)
    device.set(cv2.CAP_PROP_AUTO_WB, 0)
    success, raw = device.read()
    if success is False:
        print("Cannot read data from camera device")
        exit()
    
    h, w, _ = raw.shape
    mtx = np.matrix([[2517.792, 0., 814.045],[0., 2514.767, 567.330],[0., 0., 1.]])
    dist = np.matrix([[-0.361044, 0.154482, 0.000808, 0.000033, 0.]])
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),0,(w,h))

    # maker parameters
    markerdict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)
    markerparams = cv2.aruco.DetectorParameters_create()

    while True:
        # use for camera device
        success, raw = device.read()
        #raw = cv2.GaussianBlur(raw,(5,5),0)

        if success is False:
            print("Capture Error")
            break
        raw_gray = cv2.cvtColor(raw, cv2.COLOR_BGR2GRAY) # convert to grayscale

        # preprocessing binarization
        #raw_gray = cv2.adaptiveThreshold(raw_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 5, 5)
        _, raw_gray = cv2.threshold(raw_gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

        # undistortion by camera matrix
        raw_gray = cv2.undistort(raw_gray, mtx, dist, None, newcameramtx)
        raw_color = cv2.cvtColor(raw_gray, cv2.COLOR_GRAY2BGR) # convert to grayscale

        # find markers
        corners, ids, rejected = cv2.aruco.detectMarkers(raw_gray, markerdict, parameters=markerparams)
        if len(corners) > 0:
            for i in range(0, len(ids)):
                rvec, tvec, markerPoints = cv2.aruco.estimatePoseSingleMarkers(corners[i], 0.02, mtx, dist)
                
                if ids[i] == 19:
                    print("{}\tX : {}\tY : {}\tZ : {}".format(ids[i], tvec.reshape(-1)[0]*100, tvec.reshape(-1)[1]*100, tvec.reshape(-1)[2]*100))

                (topLeft, topRight, bottomRight, bottomLeft) = corners[i].reshape((4,2))
                topRight = (int(topRight[0]), int(topRight[1]))
                bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                topLeft = (int(topLeft[0]), int(topLeft[1]))

                cX = int((topLeft[0] + bottomRight[0]) / 2.0)
                cY = int((topLeft[1] + bottomRight[1]) / 2.0)
                cv2.circle(raw_color, (cX, cY), 4, (0, 0, 255), -1)

                cv2.aruco.drawDetectedMarkers(raw_color, corners) 
                cv2.aruco.drawAxis(raw_color, mtx, dist, rvec, tvec, 0.01) 
                cv2.putText(raw_color, str(ids[i]),(topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        

        cv2.imshow("Detected Marker",raw_color)


        key = cv2.waitKey(1)
        if key == 27:
            cv2.destroyAllWindows()
            device.release()
            break
