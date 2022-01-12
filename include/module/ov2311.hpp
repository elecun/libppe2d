/**
 * @file ov2311.hpp
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief OV2311 Camera Device with various interface(MIPI, USB3.0)
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MODULE_OV2311_HPP_
#define _MODULE_OV2311_HPP

namespace ppe { 
    class controller::iController; 
    class source::driver;
}

namespace ppe::camera {

    class ov2311 : public ppe::source::driver {
        public:
            ov2311(){ }
            virtual ~ov2311() = default;

            /**
             * @brief Set the bus object
             * 
             * @param bus bus controller to use
             */
            void set_bus(ppe::controller::iController* bus) override;

        protected:
            ppe::controller::iController* _controller = nullptr;

    }

} /* end namespace */


#endif