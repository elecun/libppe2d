
#include "task_wafer.hpp"
#include <include/spdlog/spdlog.h>
#include <opencv2/aruco.hpp>
#include <opencv2/imgproc/imgproc_c.h>

namespace ppe {

    bool task_wafer::start(){
        this->set_status(ppe::task::STATUS::RUNNING);
        return true;
    }

    bool task_wafer::stop(){
        this->set_status(ppe::task::STATUS::STOPPED);
        return false;
    }

    void task_wafer::process(cv::Mat data){
        
        cv::Mat input = data.clone();

        //1. find aruco marker
        cv::Mat aruco_marker;
        cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
        cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
        vector<vector<cv::Point2f>> markerCorners, rejectedCandidates;
        vector<int> markerIds;
        cv::aruco::detectMarkers(input, dict, markerCorners, markerIds, parameters, rejectedCandidates);

        //3. pose estimajton
        if(markerIds.size() > 0) {
            std::vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.04, _drv->camera_matrix, _drv->distortion_coeff, rvecs, tvecs);

            //2. draw markers
            cv::cvtColor(input, input, cv::COLOR_GRAY2RGB);
            //cv::aruco::drawDetectedMarkers(input, markerCorners, markerIds);
            for(int i=0; i<markerIds.size(); i++){
                if(markerIds[i]==1){
                    cv::aruco::drawAxis(input, _drv->camera_matrix, _drv->distortion_coeff, rvecs[i], tvecs[i], 0.1);
                    spdlog::info("tvec : {}\t{}", tvecs[0][0], tvecs[0][1]);
                }
                    
            }

            cv::line(input, cv::Point((int)input.cols/2, 0), cv::Point((int)input.cols/2, input.rows), cv::Scalar(0,255,0));
            cv::line(input, cv::Point(0, (int)input.rows/2), cv::Point(input.cols, (int)input.rows/2), cv::Scalar(0,255,0));
        }

        
        cv::resize(input, input, cv::Size( input.cols/2, input.rows/2 ), 0, 0, CV_INTER_NN);
        cv::imshow("marker", input);

    }

    void task_wafer::request(cv::Mat data){
        _task = async(launch::async, &task_wafer::process, this, data);
    }

    void task_wafer::wait(){
        _task.wait();
    }

} /* namespace */