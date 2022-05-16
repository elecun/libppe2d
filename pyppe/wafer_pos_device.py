
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
        # success, raw = device.read()
        raw_gray = cv2.cvtColor(raw, cv2.COLOR_BGR2GRAY) # convert to grayscale

        # undistortion by camera matrix
        raw_gray = cv2.undistort(raw_gray, mtx, dist, None, newcameramtx)

        # find markers
        corners, ids, rejected = cv2.aruco.detectMarkers(raw, markerdict, parameters=markerparams)
        if len(corners)>0:
            ids = ids.flatten()

            for (markerCorner, markerID) in zip(corners, ids):		
                corners = markerCorner.reshape((4, 2))
                (topLeft, topRight, bottomRight, bottomLeft) = corners
                # convert each of the (x, y)-coordinate pairs to integers
                topRight = (int(topRight[0]), int(topRight[1]))
                bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                topLeft = (int(topLeft[0]), int(topLeft[1]))

                cv2.line(raw, topLeft, topRight, (0, 255, 0), 2)
                cv2.line(raw, topRight, bottomRight, (0, 255, 0), 2)
                cv2.line(raw, bottomRight, bottomLeft, (0, 255, 0), 2)
                cv2.line(raw, bottomLeft, topLeft, (0, 255, 0), 2)
        

        # pose estimation


        key = cv2.waitKey(0)
        if key == 27:
            cv2.destroyAllWindows()
            device.release()
            break
