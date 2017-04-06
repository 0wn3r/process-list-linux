#include "memoryInfo.h"

const std::string info = "/proc/meminfo";

constexpr unsigned long pageToKBShift()
{
    unsigned long pageToKBShift{0};
    int i = getpagesize();
    while(i > 1024)
    {
        i >>= 1;
        pageToKBShift++;
    }
    return pageToKBShift;
}

unsigned long pagesToKB(unsigned long n)
{
    return n << pageToKBShift();
}

long getTotalMemory()
{
    std::stringstream outputstream;
    readFile(info, outputstream);

    std::string tmp;
    std::getline(outputstream, tmp);

    if (tmp.find("MemTotal") != std::string::npos)
    {
        return std::stol(tmp.substr(9));
    }

    return 0;
}