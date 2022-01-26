
#include "task_wafer.hpp"
#include <include/spdlog/spdlog.h>
#include <opencv2/aruco.hpp>

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

        //2. convert to color
        cv::Mat input_color;
        cv::cvtColor(input, input_color, cv::COLOR_GRAY2RGB);

        //3. draw
        cv::line(input_color, cv::Point((int)input_color.cols/2, 0), cv::Point((int)input_color.cols/2, input_color.rows), cv::Scalar(0,255,0));
        cv::line(input_color, cv::Point(0, (int)input_color.rows/2), cv::Point(input_color.cols, (int)input_color.rows/2), cv::Scalar(0,255,0));

        cv::imshow("wafer", input_color);
    }

    void task_wafer::request(cv::Mat data){
        _task = async(launch::async, &task_wafer::process, this, data);
    }

    void task_wafer::wait(){
        _task.wait();
    }

} /* namespace */