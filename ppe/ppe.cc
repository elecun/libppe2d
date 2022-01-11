

/**
 * @brief ppe.cc
 * @brief Precise Position Estimation Program
 * @author Byunghun Hwang<bh.hwang@iae.re.kr>
 */

//#define _USE_OV2311_


#include <libppe2d/libppe2d.hpp> //dedicated ppe library
#include <include/cxxopts.hpp>
#include <csignal>
#include <include/spdlog/spdlog.h>
#include <include/spdlog/sinks/stdout_color_sinks.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "common.hpp"
#ifdef _USE_OV2311_
    #include <module/ov2311.hpp> //camera module
    ppe::iCamera* _camera = nullptr;
#endif

/* analysis source */
enum SOURCES : int {
    CAMERA,
    IMAGE,
    VIDEO,
    UNKNOWN
};

using namespace std;

/* global variables */


/* cleanup the program */
void cleanup(int sig) {
    ppe::cleanup(sig);

    #ifdef _USE_OV2311_
        if(_camera!=nullptr){
            _camera->close();
            delete _camera;
        }
    #endif
        

    spdlog::info("Successfully terminated");
    exit(EXIT_SUCCESS);
}



/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    ppe::signal_set();
    
    int optc = 0;
    int _source = SOURCES::IMAGE;
    while((optc=getopt(argc, argv, "s:h"))!=-1)
    {
        switch(optc){
            case 's': { 
                string src = optarg;
                if(!src.compare("camera")) _source = SOURCES::CAMERA;
                else if(!src.compare("image")) _source = SOURCES::IMAGE;
                else if(!src.compare("video")) _source = SOURCES::VIDEO;
                else { _source = SOURCES::UNKNOWN; spdlog::error("{} is unknown source", src); }
            } break;
            case 'h':
            default:
                cout << fmt::format("PPE Application (built {}/{})", __DATE__, __TIME__) << endl;
                cout << "Usage: ppe [-s source] [-h]" << endl;
                exit(EXIT_FAILURE);
            break;
        }
    }


    /* source selection  */
    switch(_source){
        case SOURCES::IMAGE: { } break;
        case SOURCES::VIDEO: { } break;
        case SOURCES::CAMERA: {
            #ifdef _USE_OV2311_
                _camera = new ppe::ov2311(0, _MAX_RESOLUTION_);
                _camera->open();
            #endif
        } break;
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