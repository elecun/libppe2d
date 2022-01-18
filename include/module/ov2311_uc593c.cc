
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

using namespace std;


static bool _running = false;
namespace ppe::cmos {

    /* image capture thread function */
    void captureImage_thread(ArduCamHandle handle) {
        Uint32 rtn_val = ArduCam_beginCaptureImage(handle);
        if(rtn_val==USB_CAMERA_USB_TASK_ERROR) {
            spdlog::error("Error beginning capture({})", rtn_val);
            return;
        }

        while(_running){
            rtn_val = ArduCam_captureImage(handle);
            if(rtn_val==USB_CAMERA_USB_TASK_ERROR) {
                spdlog::error("Error beginning capture image({})", rtn_val);
                break;
            }
            else if(rtn_val>0xff){
                spdlog::error("Error beginning capture image({})", rtn_val);
            }
        }
        _running = false;
        ArduCam_endCaptureImage(handle);
        spdlog::info("Capture image thread stopped");
    }

    ov2311_uc593c::ov2311_uc593c(const char* config):_config_file(config){

    }
    
    ov2311_uc593c::~ov2311_uc593c(){

    }

    void ov2311_uc593c::set_bus(ppe::controller::iController* bus){
        _controller = bus;
    }

    bool ov2311_uc593c::open(){

        //1. Scan camera module
        ArduCamIndexinfo _camInfo[16];
        unsigned int n_cam = ArduCam_scan(_camInfo);
        spdlog::info("{} Camera Found", n_cam);

        if(n_cam==0){
            spdlog::error("Camera does not exist");
            return false;
        }

        //2. read camera configuration from file
        if(this->configure(_config_file.c_str(), _handle, _config)){
            ArduCam_setMode(_handle, CONTINUOUS_MODE);
            _capture_thread = new thread(captureImage_thread, _handle);
            //_read_thread = new thread(ov2311_uc593c::readImage_thread, _handle);
            return true;
        }
        return false;
    }

    bool ov2311_uc593c::init(){
        return false;
    }

    void ov2311_uc593c::close(){
        //_read_thread->join();
		_capture_thread->join();

        ArduCam_close(_handle);
    }

    bool ov2311_uc593c::is_valid(){
        return true;
    }

    cv::Mat ov2311_uc593c::convert(ArduCamOutData* frameData){
        cv::Mat rawImage;
        Uint8* data = frameData->pu8ImageData;
        int height, width;
        width = _config.u32Width;
        height = _config.u32Height;

        switch (_config.emImageFmtMode){
            case FORMAT_MODE_RGB:
                //rawImage = RGB565toMat(data, width, height);
                break;
            case FORMAT_MODE_RAW_D:
                /*rawImage = separationImage(data, width, height);
                switch (color_mode) {
                case RAW_RG:cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerRG2BGR);
                    break;
                case RAW_GR:cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerGR2BGR);
                    break;
                case RAW_GB:cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerGB2BGR);
                    break;
                case RAW_BG:cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerBG2BGR);
                    break;
                default:
                    cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerRG2BGR);
                    break;
                }*/
                break;
            case FORMAT_MODE_MON_D:
                //rawImage = separationImage(data, width, height);
                break;
            case FORMAT_MODE_JPG:
                //rawImage = JPGToMat(data, frameData->stImagePara.u32Size);
                break;
            case FORMAT_MODE_RAW:
                if (_config.u8PixelBytes == 2) {
                    rawImage = dBytesToMat(data, frameData->stImagePara.u8PixelBits, width, height);
                }
                else {
                    rawImage = BytestoMat(data, width, height);
                }
                break;
            case FORMAT_MODE_YUV:
                //rawImage = YUV422toMat(data, width, height);
                break;
            case FORMAT_MODE_MON:
                /*if (cameraCfg.u8PixelBytes == 2) {
                    rawImage = dBytesToMat(data, frameData->stImagePara.u8PixelBits, width, height);
                }
                else {
                    rawImage = BytestoMat(data, width, height);
                }*/
                break;
            default:
                /*if (cameraCfg.u8PixelBytes == 2) {
                    rawImage = dBytesToMat(data, frameData->stImagePara.u8PixelBits, width, height);
                }
                else {
                    rawImage = BytestoMat(data, width, height);
                }
                cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerRG2RGB);*/
                break;
            }

        return rawImage;
    }
    

    cv::Mat ov2311_uc593c::capture(){
        if(ArduCam_availableImage(_handle)>0){
            ArduCamOutData* frameData;
            Uint32 rtn_val = ArduCam_readImage(_handle, frameData);
            if(rtn_val==USB_CAMERA_NO_ERROR){
                return convert(frameData);
            }
        }

        return cv::Mat();
    }

    void ov2311_uc593c::config_board(ArduCamHandle &cameraHandle, Config config){
        uint8_t u8Buf[10];
        for (unsigned int n = 0; n < config.params[3]; n++) {
            u8Buf[n] = config.params[4 + n];
        }
        ArduCam_setboardConfig(cameraHandle, config.params[0], config.params[1], config.params[2], config.params[3], u8Buf);
    }

    bool ov2311_uc593c::configure(const char* filename, ArduCamHandle &handle, ArduCamCfg &config){
        CameraConfigs cfgs;
        memset(&cfgs, 0x00, sizeof(CameraConfigs));

        //1. read config file parse
        if(arducam_parse_config(filename, &cfgs)) {
            spdlog::error("Cannot find configuration file : {}", filename);
            return false;
        }

        // CameraParam* cam_param = &cfgs.camera_param;
        // Config* configs = cfgs.configs;
        int configs_length = cfgs.configs_length;

        // set I2C mode
        switch(cfgs.camera_param.i2c_mode){
            case 0: config.emI2cMode = I2C_MODE_8_8; break;
            case 1: config.emI2cMode = I2C_MODE_8_16; break;
            case 2: config.emI2cMode = I2C_MODE_16_8; break;
            case 3: config.emI2cMode = I2C_MODE_16_16; break;
            default: break;
        }

        // set Color format
        int color_mode = (cfgs.camera_param.format&0xff);
        switch(cfgs.camera_param.format>>8){
            case 0: config.emImageFmtMode = FORMAT_MODE_RAW; break;
            case 1: config.emImageFmtMode = FORMAT_MODE_RGB; break;
            case 2: config.emImageFmtMode = FORMAT_MODE_YUV; break;
            case 3: config.emImageFmtMode = FORMAT_MODE_JPG; break;
            case 4: config.emImageFmtMode = FORMAT_MODE_MON; break;
            case 5: config.emImageFmtMode = FORMAT_MODE_RAW_D; break;
            case 6: config.emImageFmtMode = FORMAT_MODE_MON_D; break;
            default: break;
        }

        // set other parameters
        config.u32Width = cfgs.camera_param.width;
        config.u32Height = cfgs.camera_param.height;
        config.u32I2cAddr = cfgs.camera_param.i2c_addr;
        config.u8PixelBits = cfgs.camera_param.bit_width;
        config.u32TransLvl = cfgs.camera_param.trans_lvl;

        if(config.u8PixelBits<=8){ config.u8PixelBytes = 1; }
        else if(config.u8PixelBits>8 && config.u8PixelBits<=16){
            config.u8PixelBytes = 2;
            //save_raw = true;
        }

        //2. open
        if(ArduCam_autoopen(handle, &config)==USB_CAMERA_NO_ERROR){
            for(unsigned int i=0; i<cfgs.configs_length; i++){
                uint32_t type = cfgs.configs[i].type;
                if (((type >> 16) & 0xFF) && ((type >> 16) & 0xFF) != config.usbType)
                    continue;
                switch (type & 0xFFFF) {
                case CONFIG_TYPE_REG: { ArduCam_writeSensorReg(handle, cfgs.configs[i].params[0], cfgs.configs[i].params[1]); } break;
                case CONFIG_TYPE_DELAY: { usleep(1000 * cfgs.configs[i].params[0]); } break;
                case CONFIG_TYPE_VRCMD: { config_board(handle, cfgs.configs[i]); } break;
                }
            }

            ArduCam_registerCtrls(handle, cfgs.controls, cfgs.controls_length);
        }
        else {
            spdlog::error("Cannot open camera device");
            return false;
        }

        // set framerate
        ArduCam_setCtrl(handle, "setFramerate", 5);

        return true;
    }

    cv::Mat ov2311_uc593c::dBytesToMat(Uint8* bytes, int bit_width, int width, int height) {
        unsigned char* temp_data = (unsigned char*)malloc(width * height);
        int index = 0;
        for (int i = 0; i < width * height * 2; i += 2) {
            unsigned char temp = ((bytes[i + 1] << 8 | bytes[i]) >> (bit_width - 8)) & 0xFF;
            temp_data[index++] = temp;
        }
        cv::Mat image = cv::Mat(height, width, CV_8UC1);
        memcpy(image.data, temp_data, _config.u32Height * _config.u32Width);
        free(temp_data);
        return image;
    }

    cv::Mat ov2311_uc593c::BytestoMat(Uint8* bytes, int width, int height)
    {
        cv::Mat image = cv::Mat(height, width, CV_8UC1, bytes);
        return image;
    }

}