
'''
Wafer Pose Estimation Test
'''

import cv2
from cv2 import COLOR_GRAY2BGR
import numpy as np
from cv2 import COLOR_RGB2GRAY
        

if __name__ == "__main__":

    # use image file
    raw = cv2.imread("./pyppe/data/x-1.0.jpg")
    cv2
    
    if raw.size == 0:
        print("No data")

    h, w, _ = raw.shape
    mtx = np.matrix([[2517.792, 0., 814.045],[0., 2514.767, 567.330],[0., 0., 1.]])
    dist = np.matrix([[-0.361044, 0.154482, 0.000808, 0.000033, 0.]])
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),0,(w,h))

    # maker parameters
    markerdict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_6X6_250)
    markerparams = cv2.aruco.DetectorParameters_create()

    raw_gray = cv2.cvtColor(raw, cv2.COLOR_BGR2GRAY) # convert to grayscale
    raw_gray = cv2.undistort(raw_gray, mtx, dist, None, newcameramtx)
    raw_color = cv2.cvtColor(raw_gray, cv2.COLOR_GRAY2BGR) # convert to grayscale


    corners, ids, rejected = cv2.aruco.detectMarkers(raw_gray, markerdict, parameters=markerparams)

    if len(corners) > 0:
        for i in range(0, len(ids)):
            rvec, tvec, markerPoints = cv2.aruco.estimatePoseSingleMarkers(corners[i], 0.02, mtx, dist)

            print("ID : {} = {}".format(ids[i], tvec))

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



    # print(corners[0])
    # if len(corners)>0:
    #     ids = ids.flatten()

    #     for (markerCorner, markerID) in zip(corners, ids):		
    #         corner = markerCorner.reshape((4, 2))
    #         (topLeft, topRight, bottomRight, bottomLeft) = corner
            
    #         # convert each of the (x, y)-coordinate pairs to integers
    #         topRight = (int(topRight[0]), int(topRight[1]))
    #         bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
    #         bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
    #         topLeft = (int(topLeft[0]), int(topLeft[1]))

    #         cv2.line(raw_color, topLeft, topRight, (0, 255, 0), 2)
    #         cv2.line(raw_color, topRight, bottomRight, (0, 255, 0), 2)
    #         cv2.line(raw_color, bottomRight, bottomLeft, (0, 255, 0), 2)
    #         cv2.line(raw_color, bottomLeft, topLeft, (0, 255, 0), 2)

    #         cX = int((topLeft[0] + bottomRight[0]) / 2.0)
    #         cY = int((topLeft[1] + bottomRight[1]) / 2.0)
    #         cv2.circle(raw_color, (cX, cY), 4, (0, 0, 255), -1)

    #         if markerID is not None:
    #             print("Found Marker ID : {}".format(markerID))
    #             print("Corner : {}".format(corner))
    #             print(type(corner))
    #             rvec, tvec = cv2.aruco.estimatePoseSingleMarkers(corner, 0.02, mtx, dist)
            
    #             #cv2.aruco.drawAxis(raw_color, mtx, dist, rvec, tvec, 0.01)
    #             cv2.aruco.drawAxis(raw_color, mtx, dist, rvec, tvec, 0.01) 

    #             # draw the ArUco marker ID on the frame
    #             cv2.putText(raw_color, str(markerID),(topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        
        cv2.imshow("Detected Marker",raw_color)

    while True:
        key = cv2.waitKey(0)
        if key == 27:
            cv2.destroyAllWindows()
            break
