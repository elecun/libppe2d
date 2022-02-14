

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
#include "common.hpp"
#include <memory>
#include <ctime>
#include <vector>
#include <include/cxxopts.hpp>
#include <future>

#include <core/driver.hpp>
#include <include/core/iController.hpp>

/* to use camera device for ov2311 with USB3.0 or MIPI interface */
//#include <module/ov2311_uc593c.hpp>
#include <module/ov2311_uc762c.hpp>

/* for multi tasking */
#include "task_wafer.hpp"
#include "task_tool.hpp"

using namespace std;

/* global variables */
ppe::source::driver* g_source = nullptr;
ppe::taskmanager* g_taskmanager = nullptr;

/* calc fps */
int show_fps(){
    static int frame_count = 0;
	static time_t beginTime = time(NULL);
    frame_count++;
	if((time(NULL)-beginTime)>=1)
	{
		beginTime = time(NULL);
        spdlog::info("Fps : {}", frame_count);
		frame_count = 0;
	}
    return frame_count;
}

/* terminate the program */
    void destroy() {
        if(g_source){
            g_source->close();
            delete g_source;
        }
        cv::destroyAllWindows();

        if(g_taskmanager!=nullptr){
            delete g_taskmanager;
        }
            
        exit(EXIT_SUCCESS);
    }

    /* handling for signal event */
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

        destroy();
            
        spdlog::info("Successfully terminated");
        exit(EXIT_SUCCESS);
    }

    /* signal setting into main thread */
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
    
    /* parse arguments */
    int optc = 0;
    int camera_id = 0;
    ppe::SOURCE _source = ppe::SOURCE::CAMERA;
    ppe::CMOS _cmos = ppe::CMOS::OV2311_UC593C; //OV2311+USB3.0
    ppe::EXP _exp = ppe::EXP::UNKNOWN;
    
    while((optc=getopt(argc, argv, "s:c:i:e:h"))!=-1)
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
                else if(!cmos.compare("ov2311_uc762c")) _cmos = ppe::CMOS::OV2311_UC762C;
                else { 
                    _cmos = ppe::CMOS::UNKNOWN; 
                    spdlog::warn("Unknown CMOS with Interface : {}", cmos);
                }
            }
            break;
            case 'i':{
                camera_id = atoi(optarg);
                spdlog::info("Camera ID : {}", camera_id);
            } break;
            case 'e':{
                string exp = optarg;
                if(!exp.compare("mcl_pf")) _exp = ppe::EXP::MCL_PF;
                else {
                    _exp = ppe::EXP::UNKNOWN;
                    spdlog::warn("Unknown Experimental option");
                }

            } break;

            /* help & unkown options */
            case 'h':
            default:
                cout << fmt::format("PPE Application (built {}/{})", __DATE__, __TIME__) << endl;
                cout << "Usage: ppe [-s source<camera|image|video>] [-c CMOS sensor <ov2311_uc593c|ov2311_uc621b|ov2311_uc762c>] [-i camera id] [-h]" << endl;
                destroy();
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
                    #if defined(_MODULE_OV2311_UC593C_HPP_)
                    g_source = new ppe::cmos::ov2311_uc593c("./OV2311_MIPI_2Lane_RAW10_10b_1600x1300.cfg", 30);
                    if(g_source->open()) {
                        _ready = true;
                    }
                    #endif
                } break;
                case ppe::CMOS::OV2311_UC621B: {
                    
                } break;
                case ppe::CMOS::OV2311_UC762C: {
                    g_source = new ppe::cmos::ov2311_uc762c(camera_id);
                    if(g_source->open()){
                        _ready = true;
                    }
                    else
                        spdlog::error("Camera module open error");
                } break;
                case ppe::CMOS::OV2311_UVC:{

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
        bool rectified = false;
        cv::Mat undistorMapx, undistorMapy; //undistortion map
        cv::Mat newCameraMatrix;    //optimal camera matrix

        /* create task & run */
        g_taskmanager = new ppe::taskmanager();
        //g_taskmanager->add("tool", new ppe::task_tool());
        g_taskmanager->add("wafer", new ppe::task_wafer(g_source));
        g_taskmanager->start();

        while(1){
            /* image capture & preprocessing */
            if(g_source->is_valid()){
                cv::Mat raw = g_source->capture();
                cv::Mat rectified_raw;
                if(!raw.empty()){
                    if(!rectified){
                        //1. calc undistortion map
                        //cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(g_source->camera_matrix, g_source->distortion_coeff, cv::Size(g_source->width, g_source->height), 1);
                        cv::initUndistortRectifyMap(g_source->camera_matrix, g_source->distortion_coeff, cv::Mat(), g_source->camera_matrix, cv::Size(g_source->width, g_source->height), CV_32FC1, undistorMapx, undistorMapy);
                        rectified = true;
                    }
                    else {
                        //1. generate undist image with linear interpolation
                        cv::remap(raw, rectified_raw, undistorMapx, undistorMapy, cv::INTER_LINEAR);

                        // cv::Point outputPoint;
                        // outputPoint.x = undistorMapx.at<float>(raw.cols/2 , raw.rows/2);
                        // outputPoint.y = undistorMapy.at<float>(raw.cols/2, raw.rows/2);
                        // spdlog::info("{},{} --> {},{}", raw.cols/2, raw.rows/2, outputPoint.x, outputPoint.y);

                        /* image data transfer to related task */
                        g_taskmanager->request_all(rectified_raw);
                        g_taskmanager->wait();

                        //check fps
                        show_fps();

                    }
                }

                int key = cv::waitKey(10);
                switch(key){
                    case 'c':
                    case 'C':
                        if(!raw.empty()){
                            time_t timer = time(nullptr);
                            struct tm* t = localtime(&timer);
                            string filename = fmt::format("capture_{}-{}-{}T{}-{}-{}.jpg", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
                            cv::imwrite(filename, raw);
                            spdlog::info("saved : {}", filename);
                        }
                    break;

                    case 27: { //Escape
                        destroy();
                    } break;
                }
            }
        }
    }
    else {
        destroy();
    }

    ::pause();

    destroy();
    return EXIT_SUCCESS;

}