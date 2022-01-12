/**
 * @file uc593c.hpp
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief MIPI to USB3.0 bus conversion controller
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MODULE_UC593_HPP_
#define _MODULE_US593_HPP_

#include <include/core/iController.hpp>

namespace ppe::controller {

    class uc593c : public iController {
        public:
            uc593c() = default;
            virtual ~uc593c() = default;
    }; 

} /* end namespace */

#endif