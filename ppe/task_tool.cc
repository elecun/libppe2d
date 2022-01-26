
#include "task_tool.hpp"
#include <include/spdlog/spdlog.h>
#include <future>

namespace ppe {

    bool task_tool::start(){
        this->set_status(ppe::task::STATUS::RUNNING);
        return false;
    }

    bool task_tool::stop(){
        this->set_status(ppe::task::STATUS::STOPPED);
        return true;
    }

    void task_tool::process(cv::Mat data){
        cv::Mat input = data.clone();
        cv::imshow("tool", input);
    }

    void task_tool::request(cv::Mat data){
        _task = async(launch::async, &task_tool::process, this, data);

        // if(!_working){
        //     _working = true;
        //     auto task = async(launch::async, &task_tool::process, this, data);
        //     task.get();
        //     _working = false;
        // }
        // else
        //     spdlog::info("task tool working... passed");
    }

    void task_tool::wait(){
        _task.wait();
    }


} /* namespace */