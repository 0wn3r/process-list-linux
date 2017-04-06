#include "cpuInfo.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "../utility/readFile.h"

const std::string stat = "/proc/stat";

CpuInfo getCPUInfo() {
    std::stringstream info;
    readFile(stat, info);

    unsigned long long t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    std::string foo;

    info >> foo;
    info >> t0;
    info >> t1;
    info >> t2;
    info >> t3;
    info >> t4;
    info >> t5;
    info >> t6;
    info >> t7;
    info >> t8;
    info >> t9;


    return {t0, t1, t2, t3, t4, t5, t6, t7, t8, t9};
}