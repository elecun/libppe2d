/**
 * @file ov2311_uc593c.hpp
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief OV2311 Monochrome CMOS Sensor with UC593C USB 3.0 Controller Interface
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MODULE_OV2311_UC593C_HPP_
#define _MODULE_OV2311_UC593C_HPP_

#include <string>
#include <memory>
#include <core/driver.hpp>

#include "module/ArduCamLib.h"

using namespace std;

namespace ppe::controller { class iController; }
namespace ppe::cmos {

    class ov2311_uc593c : public ppe::source::driver {
        public:
            ov2311_uc593c(const char* config);
            virtual ~ov2311_uc593c();

            void set_bus(ppe::controller::iController* bus) override;
            bool open() override;
            void close() override;
            bool is_valid() override;
            cv::Mat capture() override;

        protected:
            ppe::controller::iController* _controller = nullptr;

        private:
            string _config_file {""};

            ArduCamCfg cameraCfg;
            ArduCamIndexinfo pUsbIdxArray[16];
            int _camera_num = 0;

    };

} /* end namespace */


#endif