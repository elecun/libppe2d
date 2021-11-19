



/**
 * @brief ppe.cc
 * @brief Precise Position Estimation Program
 * @author Byunghun Hwang<bh.hwang@iae.re.kr>
 */


#include <libppe2d/libppe2d.hpp> //dedicated ppe library
#include <module/ov2311.hpp> //camera module
#include <include/cxxopts.hpp>
#include <csignal>

#include "instance.h"

using namespace std;


/* main entry */
int main(int argc, char** argv){
    spdlog::stdout_color_st("console");
    signal_set();
    

}