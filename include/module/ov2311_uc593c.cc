
#include "ov2311_uc593c.hpp"
#include <core/iController.hpp>
#include <spdlog/spdlog.h>

#include <opencv2/opencv.hpp>
#include <thread>
#include <time.h>
#include <iostream>
#include <istream>
#include <string>
#include <sys/types.h> 
#include <sys/stat.h> 
#include "arducam_config_parser.h"

using namespace std;


namespace ppe::cmos {

    ov2311_uc593c::ov2311_uc593c(const char* config):_config_file(config){

    }
    
    ov2311_uc593c::~ov2311_uc593c(){

    }

    void ov2311_uc593c::set_bus(ppe::controller::iController* bus){
        _controller = bus;
    }

    bool ov2311_uc593c::open(){
        _camera_num = ArduCam_scan(pUsbIdxArray);
        spdlog::info("Camera Num : {}", _camera_num);
        char serial[16];
        unsigned char *u8TmpData;
        for (int i = 0; i < _camera_num; i++) {
            u8TmpData = pUsbIdxArray[i].u8SerialNum;
            sprintf(serial, "%c%c%c%c-%c%c%c%c-%c%c%c%c",
                u8TmpData[0], u8TmpData[1], u8TmpData[2], u8TmpData[3],
                u8TmpData[4], u8TmpData[5], u8TmpData[6], u8TmpData[7],
                u8TmpData[8], u8TmpData[9], u8TmpData[10], u8TmpData[11]);
            spdlog::info("Index:{}\tSerial:{}",pUsbIdxArray[i].u8UsbIndex, serial);
        }


        return false;
    }

    void ov2311_uc593c::close(){
        
    }

    bool ov2311_uc593c::is_valid(){
        return true;
    }

    cv::Mat ov2311_uc593c::capture(){

        return cv::Mat();
    }
}