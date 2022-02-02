

#ifndef _PPE_TASK_MCL_PF_HPP_
#define _PPE_TASK_MCL_PF_HPP_


#include <include/core/task.hpp>
#include <opencv2/opencv.hpp>
#include <future>

using namespace std;

namespace ppe {

    class task_mcl_pf : public ppe::task {
        public:
            task_mcl_pf() = default;
            virtual ~task_mcl_pf() = default;
    };

#endif