

#ifndef _PPE_COMMON_HPP_
#define _PPE_COMMON_HPP_

#include <csignal>
#include <include/spdlog/spdlog.h>
#include <include/spdlog/sinks/stdout_color_sinks.h>


/* cleanup the program */
void cleanup(int sig) {
  switch(sig)
  {
    case SIGSEGV: { spdlog::warn("Segmentation violation"); } break;
    case SIGABRT: { spdlog::warn("Abnormal termination"); } break;
    case SIGKILL: { spdlog::warn("Process killed"); } break;
    case SIGBUS: { spdlog::warn("Bus Error"); } break;
    case SIGTERM: { spdlog::warn("Termination requested"); } break;
    case SIGINT: { spdlog::warn("interrupted"); } break;
    default:
      spdlog::info("Cleaning up the program");
  }

    spdlog::info("Successfully terminated");
    exit(EXIT_SUCCESS);
  
}


/* Signal Responses */
void signal_set(){
    const int signals[] = { SIGINT, SIGTERM, SIGBUS, SIGKILL, SIGABRT, SIGSEGV };

    for(const int& s:signals)
        signal(s, cleanup);

    //signal masking
    sigset_t sigmask;
    if(!sigfillset(&sigmask)){
        for(int signal:signals)
            sigdelset(&sigmask, signal); //delete signal from mask
    }
    else {
        spdlog::error("Signal Handling Error");
        cleanup(0);
    }

    if(pthread_sigmask(SIG_SETMASK, &sigmask, nullptr)!=0){ // signal masking for this thread(main)
        spdlog::error("Signal Masking Error");
        cleanup(0);
    }
}

#endif