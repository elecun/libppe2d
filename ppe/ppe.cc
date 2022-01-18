

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
#include <module/ov2311_uc593c.hpp>
#include <module/uc593c.hpp> //for usb 3.0 bus controller with uc621 MIPI controller
#include <module/uc621b.hpp> //for MIPI bus controller


/* global variables */
ppe::source::driver* g_source = nullptr;

using namespace std;


/* cleanup the program */
void cleanup(int sig) {
    if(g_source){
        g_source->close();
        delete g_source;
    }
    cv::destroyAllWindows();

    ppe::cleanup(sig);
    exit(EXIT_SUCCESS);
}


/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    ppe::signal_set();
    
    /* parse arguments */
    int optc = 0;
    ppe::SOURCE _source = ppe::SOURCE::CAMERA;
    ppe::CMOS _cmos = ppe::CMOS::OV2311_UC593C; //OV2311+USB3.0
    
    while((optc=getopt(argc, argv, "s:c:h"))!=-1)
    {
        switch(optc){
            /* source selection */
            case 's': {
                string src = optarg;
                if(!src.compare("camera")) _source = ppe::SOURCE::CAMERA;
                else if(!src.compare("image")) _source = ppe::SOURCE::IMAGE;
                else if(!src.compare("video")) _source = ppe::SOURCE::VIDEO;
                else { 
                    _source = ppe::SOURCE::UNKNOWN; 
                    spdlog::warn("Unknown input source : {}", src); }
            } break;

            /* CMOS sensor model */
            case 'c': {
                string cmos = optarg;
                if(!cmos.compare("ov2311_uc593c")) _cmos = ppe::CMOS::OV2311_UC593C;
                else if(!cmos.compare("ov2311_uc621b")) _cmos = ppe::CMOS::OV2311_UC621B;
                else { 
                    _cmos = ppe::CMOS::UNKNOWN; 
                    spdlog::warn("Unknown CMOS with Interface : {}", cmos);
                }
            }
            break;

            /* help & unkown options */
            case 'h':
            default:
                cout << fmt::format("PPE Application (built {}/{})", __DATE__, __TIME__) << endl;
                cout << "Usage: ppe [-s source<camera|image|video>] [-c CMOS sensor <ov2311_uc593c|ov2311_uc621b>] [-h]" << endl;
            break;
        }
    }


    /* source selection  */
    bool _ready = false;    //ready to open
    switch(_source){
        case ppe::SOURCE::IMAGE: { } break;
        case ppe::SOURCE::VIDEO: { } break;
        case ppe::SOURCE::CAMERA: {

            /* select cmos sensor */
            switch(_cmos){
                case ppe::CMOS::OV2311_UC593C: {
                    g_source = new ppe::cmos::ov2311_uc593c("./OV2311_MIPI_2Lane_RAW10_10b_1600x1300.cfg");
                    if(g_source->open()) {
                            _ready = true;
                    }
                } break;
                case ppe::CMOS::OV2311_UC621B: {
                    
                } break;
                case ppe::CMOS::OV2311_UVC: {

                } break;
                case ppe::CMOS::UNKNOWN: {

                } break;
            }

        } break;
        case ppe::SOURCE::UNKNOWN: {

        } break;
    }

    if(_ready){
        /* processing */
        while(1){
            /* image processing */
            if(g_source->is_valid()){
                cv::Mat final = g_source->capture();
                cv::imshow("View", final);
                cv::waitKey(1);
            }
        }
    }
    else {
        cleanup(SIGTERM);
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