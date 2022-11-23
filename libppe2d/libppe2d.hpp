/**
 * @file libppe2d.hpp
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief Precision 2D Pose Estimation Algorithm & Interface Library
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _LIB_PPE2D_HPP_
#define _LIB_PPE2D_HPP_

#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

namespace libppe2d {
    /**
     * @brief type definitions
     * 
     */
    #ifndef _pos2d
    typedef struct _pos2d {
        double x = 0.0;
        double y = 0.0;
    } pos2d;
    #endif

    #ifndef _pos3d
    typedef struct _pos3d {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    } pos3d;
    #endif

    #ifndef _wafer_param
    typedef struct _wafer_param {
        double diameter_inch = 12.0;
    } wafer_parameter;
    #endif

    #ifndef _camera_param
    typedef struct _camera_param {
        double fx, fy;
        double cx, cy;
        double dist_coeff[5] = {0.0, };
    } camera_param;
    #endif

    /**
     * @brief pre-defined parameters
     * 
     */
    #define _CAM_ID_0   0   //Camera ID 0 : Wafer viewpoint
    #define _CAM_ID_1   1   //Camera ID 1 : Fork viewpoint
    

    /**
     * @brief camera control interface functions
     */

    /**
     * @brief Set the camera model name
     * 
     * @param model_name 
     */
    //void set_camera_model(const char* model_name = "ov2311_uc762c");

    /**
     * @brief start capture from 2 cameras
     */
    void start_capture();

    /**
     * @brief stop capture from 2 cameras
     * 
     * @return string captured filemname
     */
    string stop_capture();
    
    /**
     * @brief 
     * 
     * @param video_filename input video filename
     * @return std::vector<pair<double, pos2d>> time and coordinates
     */
    std::vector<pair<double, pos2d>> compute_markers_ppe(const char* video_filename);
    std::vector<pair<double, pos2d>> compute_fork_ppe(const char* video_filename);

} /* namespace */

# endif