

/**
 * @file ppe.cc
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief PPE Application with universal ppe library 2D
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <libppe2d/libppe2d.hpp> //dedicated ppe library
#include <csignal>
#include <include/spdlog/spdlog.h>
#include <include/spdlog/sinks/stdout_color_sinks.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "common.hpp"
#include <memory>
#include "include/cxxopts.hpp"

#include <core/driver.hpp>
#include <include/core/iController.hpp>

/* to use camera device for ov2311 with USB3.0 or MIPI interface */
#if defined(USE_SOURCE_CAMERA)
    #if defined(USE_OV2311)
        #include <module/ov2311.hpp>
    #endif

    #if defined(USE_BUS_CONTROLLER)
        #if defined(USE_UC593C)
            #include <module/uc593c.hpp>
        #endif
    #endif
#endif

/* global variables */
ppe::source::driver* g_source = nullptr;


/* input source */
enum SOURCES : int {
    UNKNOWN = 0,
    CAMERA,
    IMAGE,
    VIDEO
};

using namespace std;

/* cleanup the program */
void cleanup(int sig) {
    ppe::cleanup(sig);
    exit(EXIT_SUCCESS);
}



/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    ppe::signal_set();
    
    /* parse arguments */
    int optc = 0;
    int _source = SOURCES::IMAGE;
    while((optc=getopt(argc, argv, "s:p:h"))!=-1)
    {
        switch(optc){
            /* source selection */
            case 's': {
                string src = optarg;
                if(!src.compare("camera")) _source = SOURCES::CAMERA;
                else if(!src.compare("image")) _source = SOURCES::IMAGE;
                else if(!src.compare("video")) _source = SOURCES::VIDEO;
                else { _source = SOURCES::UNKNOWN; spdlog::error("{} is unknown source.", src); }
            } break;

            /* read camera parameters */
            case 'p': {
                string _cam_param = optarg;
                
            } break;

            /* help & unkown options */
            case 'h':
            default:
                cout << fmt::format("PPE Application (built {}/{})", __DATE__, __TIME__) << endl;
                cout << "Usage: ppe [-s source<camera|image|video>] [-p parameter_file] [-h]" << endl;
                exit(EXIT_FAILURE);
            break;
        }
    }


    /* source selection  */
    switch(_source){
        case SOURCES::IMAGE: { } break;
        case SOURCES::VIDEO: { } break;
        case SOURCES::CAMERA: {
            #if defined(USE_SOURCE_CAMERA)
                #if defined(USE_OV2311)
                    g_source = new ppe::camera::ov2311();
                    #if defined(USE_UC593C)
                        g_source->set_bus(new ppe::controller::uc593c);
                    #endif
                    g_source->open();
                #endif
            #endif
        } break;
    }

    /* image processing */
    if(g_source->valid()){
        
    }

    

    /* processing */

    // 1. undistortion
    //cv::Mat map1, map2;
    //cv::initUndistortRectifyMap(_camera->_camera_matrix, _camera->_distortion_coeff, cv::Mat(), _camera->_camera_matrix, cv::Size(1600, 1300), CV_32FC1, map1, map2);

    //2. aruco marker detection
    // cv::Mat aruco_marker;
    // cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);
    // //cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::generateCustomDictionary(1, 5);
    // cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();


    ::pause();

    cleanup(0);
    return EXIT_SUCCESS;

}