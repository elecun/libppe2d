
/**
 * @brief common header file
 * @file    common.hpp
 * @author Byunghun Hwang@iae.re.kr
 */


#include <csignal>
#include <include/spdlog/spdlog.h>


namespace ppe {

    /* input source */
    enum class SOURCE : int {
        UNKNOWN,
        CAMERA,
        IMAGE,
        VIDEO
    };

    /* camera bus controllers */
    enum class BUS : int {
        UNKNOWN,
        MIPI,
        USB, //USB General (UVC)
        USB3
    };

    /* CMOS with Bus Interface */
    enum class CMOS : int {
        UNKNOWN,
        OV2311_UC593C,
        OV2311_UC621B,
        OV2311_UVC,
        OV2311_UC762C
    };

}

