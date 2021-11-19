

/**
 * @file    ov2311.hpp
 * @brief   OV2311 Camera module (arducam ov2311 with MIPI interface)
 * @author  Byunghun Hwang<bh.hwang@iae.re.kr>
 */

#ifndef _INCLUDE_MODULE_OV2311_HPP_
#define _INCLUDE_MODULE_OV2311_HPP_

#include <core/icamera.hpp>
#include <linux/v4l2-controls.h>

namespace ppe {
    class ov2311 : public ppe::icamera {

        public:
        ov2311(){

        }
        virtual ~ov2311(){

        }

    }; /* class */
} /* namespace */


#endif