
/**
 * @file task_wafer.hpp
 * @author Byunghun Hwang(bh.hwagn@iae.re.kr)
 * @brief wafer pose estimation task
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PPE_TASK_WAFER_HPP_
#define _PPE_TASK_WAFER_HPP_

#include <include/core/task.hpp>
#include <opencv2/opencv.hpp>
#include <condition_variable>

using namespace std;

namespace ppe {

    class task_wafer : public ppe::task {
        public:
            task_wafer() = default;
            virtual ~task_wafer() = default;

            virtual bool start();
            virtual bool stop();
            virtual void process(cv::Mat data);
            virtual void request(cv::Mat data);

        private:
            bool is_ready();
            

    }; 

} /* namespace */

#endif