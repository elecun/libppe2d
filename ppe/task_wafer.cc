
#include "task_wafer.hpp"

namespace ppe {

    bool task_wafer::start(){
        // create thread
        this->_thread = new thread(&task_wafer::process, this);
        this->_handle = this->_thread->native_handle();
    }

    bool task_wafer::stop(){
        if(!pthread_cancel(this->_handle))
            return true;
        return false;
    }

    void task_wafer::process(cv::Mat data){
        std::lock_guard<std::mutex> lk(this->_lock);
        this->_cv.wait(lk);

        lk.unlock();
    }

    void task_wafer::request(cv::Mat data){
        std::unique_lock<std::mutex> lk(this->_lock);
    }

    bool task_wafer::is_ready(){
        return true;
    }


} /* namespace */