
/**
 * @brief Precise Position Estimation with static images
 * @file    ppe_image.cc
 * @author  Byunghun Hwang<bh.hwang@iae.re.kr>
 */


#include <libppe2d/libppe2d.hpp>
#include <include/cxxopts.hpp>
#include <csignal>
#include <include/spdlog/spdlog.h>
#include <include/spdlog/sinks/stdout_color_sinks.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "common.hpp"

using namespace std;


/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    ppe::signal_set();
    
    int optc = 0;
    while((optc=getopt(argc, argv, "d"))!=-1)
    {
        switch(optc){
            case 'd': { } break; /* camera device */
            case 'p': { } break; /* preview */
            case 'h':
            default:
                cout << fmt::format("PPE Application (built {}/{})", __DATE__, __TIME__) << endl;
                cout << "Usage: ppe [-p]" << endl;
                exit(EXIT_FAILURE);
            break;
        }
    }

    //camera device
    if(!_camera){
        _camera = new ppe::ov2311(0, _MAX_RESOLUTION_);
        _camera->open();

        //undistortion
        cv::Mat map1, map2;
        cv::initUndistortRectifyMap(_camera->_camera_matrix, _camera->_distortion_coeff, cv::Mat(), _camera->_camera_matrix, cv::Size(1600, 1300), CV_32FC1, map1, map2);

        cv::Mat aruco_marker;
        cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);
        //cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::generateCustomDictionary(1, 5);
        cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();

        while(1){
            //cv::Mat* image = _camera->capture();
            _camera->capture_n();

            //display lines to recognize the center of image
            // cv::line(*image, cv::Point(_camera->getResolution()->width/2, 0), cv::Point(_camera->getResolution()->width/2, _camera->getResolution()->height), cv::Scalar(0,255,0), 1);
            // cv::line(*image, cv::Point(0, _camera->getResolution()->height/2), cv::Point(_camera->getResolution()->width, _camera->getResolution()->height/2), cv::Scalar(0,255,0), 1);
            
            // // undistortion
            // cv::Mat undist;
            // cv::remap(*image, undist, map1, map2, CV_INTER_LINEAR);

            // //convert to grayscale image
            // cv::Mat undist_gray;
            // cv::cvtColor(undist, undist_gray, CV_BGR2GRAY);

            // vector<vector<cv::Point2f>> markerCorners, rejectedCandidates;
            // vector<int> markerIds;

            // cv::aruco::detectMarkers(undist_gray, dict, markerCorners, markerIds, parameters, rejectedCandidates);

            // spdlog::info("Found markers : {}", markerCorners.size());
            // cv::aruco::drawDetectedMarkers(undist, markerCorners);

            // cv::imshow("Camera View", undist);
            cv::waitKey(1);
            //image->release();
        }
    }



    ::pause();

    cleanup(0);
    return EXIT_SUCCESS;

}