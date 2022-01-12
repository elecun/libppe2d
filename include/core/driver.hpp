/**
 * @file driver.hpp
 * @author Byunghun Hwang (bh.hwang@iae.re.kr)
 * @brief Image Source Driver (camera|image|video)
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#ifndef _CORE_SOURCE_DRIVER_HPP_
#define _CORE_SOURCE_DRIVER_HPP_


namespace ppe::source {

    class driver {
        public:
            driver() { };
            virtual ~driver(){ };

            /**
             * @brief set in
             * 
             * @param bus 
             */
            virtual void set_bus(ppe::controller::iController* bus) = 0;
    };
    
} /* end namespace */

#endif