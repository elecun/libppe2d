



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
        while(1){
            spdlog::info("capture");
            cv::Mat* image = _camera->capture();

            //display lines to recognize the center of image
            cv::line(*image, cv::Point(_camera->getResolution()->width/2, 0), cv::Point(_camera->getResolution()->width/2, _camera->getResolution()->height), cv::Scalar(255,0,0), 1);
            cv::line(*image, cv::Point(0, _camera->getResolution()->height/2), cv::Point(_camera->getResolution()->width, _camera->getResolution()->height/2), cv::Scalar(255,0,0), 1);

            //image undistortion
            //cv::undistort(*image, *image)

            cv::imshow("Camera View", *image);
            cv::waitKey(1);
            image->release();
        }
    }



    ::pause();

    cleanup(0);
    return EXIT_SUCCESS;

}