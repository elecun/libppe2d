



/**
 * @brief ppe.cc
 * @brief Precise Position Estimation Program
 * @author Byunghun Hwang<bh.hwang@iae.re.kr>
 */


#include <libppe2d/libppe2d.hpp> //dedicated ppe library
#include <module/ov2311.hpp> //camera module
#include <include/cxxopts.hpp>
#include <csignal>
#include <include/spdlog/spdlog.h>
#include <include/spdlog/sinks/stdout_color_sinks.h>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;

ppe::iCamera* _camera = nullptr;


/* cleanup the program */
void cleanup(int sig) {
    switch(sig)
    {
        case SIGSEGV: { spdlog::warn("Segmentation violation"); } break;
        case SIGABRT: { spdlog::warn("Abnormal termination"); } break;
        case SIGKILL: { spdlog::warn("Process killed"); } break;
        case SIGBUS: { spdlog::warn("Bus Error"); } break;
        case SIGTERM: { spdlog::warn("Termination requested"); } break;
        case SIGINT: { spdlog::warn("interrupted"); } break;
        default:
        spdlog::info("Cleaning up the program");
    }

    if(_camera!=nullptr){
        _camera->close();
        delete _camera;
    }
        

    spdlog::info("Successfully terminated");
    exit(EXIT_SUCCESS);
}


/* Signal Responses */
void signal_set(){
    const int signals[] = { SIGINT, SIGTERM, SIGBUS, SIGKILL, SIGABRT, SIGSEGV };

    for(const int& s:signals)
        signal(s, cleanup);

    //signal masking
    sigset_t sigmask;
    if(!sigfillset(&sigmask)){
        for(int signal:signals)
            sigdelset(&sigmask, signal); //delete signal from mask
    }
    else {
        spdlog::error("Signal Handling Error");
        cleanup(0);
    }

    if(pthread_sigmask(SIG_SETMASK, &sigmask, nullptr)!=0){ // signal masking for this thread(main)
        spdlog::error("Signal Masking Error");
        cleanup(0);
    }
}


/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    signal_set();
    
    int optc = 0;
    while((optc=getopt(argc, argv, "ph"))!=-1)
    {
        switch(optc){
            case 'c': { } break; /* camera device */
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
            cv::Mat* image = _camera->capture();

            //display lines to recognize the center of image
            cv::line(*image, cv::Point(_camera->getResolution()->width/2, 0), cv::Point(_camera->getResolution()->width/2, _camera->getResolution()->height), cv::Scalar(0,255,0), 1);
            cv::line(*image, cv::Point(0, _camera->getResolution()->height/2), cv::Point(_camera->getResolution()->width, _camera->getResolution()->height/2), cv::Scalar(0,255,0), 1);
            
            // undistortion
            cv::Mat undist;
            cv::remap(*image, undist, map1, map2, CV_INTER_LINEAR);

            //convert to grayscale image
            cv::Mat undist_gray;
            cv::cvtColor(undist, undist_gray, CV_BGR2GRAY);

            vector<vector<cv::Point2f>> markerCorners, rejectedCandidates;
            vector<int> markerIds;

            cv::aruco::detectMarkers(undist_gray, dict, markerCorners, markerIds, parameters, rejectedCandidates);

            spdlog::info("Found markers : {}", markerCorners.size());
            cv::aruco::drawDetectedMarkers(undist, markerCorners);

            // for(auto& marker:markerCorners){
            //     for(auto& point:marker){
            //         cv::circle(undist, point, 1, cv::Scalar(0,0,255));
            //     }
            // }

            cv::imshow("Camera View", undist);
            cv::waitKey(1);
            image->release();
        }
    }



    ::pause();

    cleanup(0);
    return EXIT_SUCCESS;

}