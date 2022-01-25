/**
 * @file task_tool.hpp
 * @author Byunghun Hwang(bh.hwagn@iae.re.kr)
 * @brief tool pose estimation thread
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PPE_TASK_TOOL_HPP_
#define _PPE_TASK_TOOL_HPP_

#include <include/core/task.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

namespace ppe {

    class task_tool : public ppe::task {
        public:
            task_tool() = default;
            virtual ~task_tool() = default;

            virtual bool start();
            virtual bool stop();
            virtual void process(cv::Mat data);
            virtual void request(cv::Mat data);

        protected:
           

    }; 
} /* namespace */

#endif