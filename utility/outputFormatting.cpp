#include "outputFormatting.h"

void sortProcesses(std::vector<ProcessInfo> &info, sortBy sorting)
{
    switch (sorting)
    {
        case sortBy::CPU:
            std::sort(info.begin(), info.end(), 
                        [](const ProcessInfo &a, const ProcessInfo &b) -> bool
                        {
                            return a.cpu_percent > b.cpu_percent;
                        });
            break;
        case sortBy::PID:
            std::sort(info.begin(), info.end(), 
                        [](const ProcessInfo &a, const ProcessInfo &b) -> bool
                        {
                            return a.pid < b.pid;
                        });
            break;
        case sortBy::MEM:
            std::sort(info.begin(), info.end(), 
                        [](const ProcessInfo &a, const ProcessInfo &b) -> bool
                        {
                            return a.resident > b.resident;
                        });
            break;
        case sortBy::NONE:
            std::sort(info.begin(), info.end(), 
                        [](const ProcessInfo &a, const ProcessInfo &b) -> bool
                        {
                            return a.cpu_percent > b.cpu_percent;
                        });
            break;
    }
}

std::string formatComm(const char *cmd)
{
    std::string comm = cmd;
    comm = comm.substr(1);
    comm = comm.substr(0, comm.size() - 1);
    return comm;
}

std::string formatHeader()
{
    std::stringstream outputstream;

    outputstream << std::right << std::fixed << std::setfill(' ');

    outputstream << std::setw(5) << "PID"
        << std::setw(8) << "VIRT"
        << std::setw(7) << "RES"
        << std::setw(7) << "SHR"
        << std::setw(2) << "S"
        << std::setw(6) << "%CPU"
        << std::setw(6)  << "%MEM"
        << std::setw(38) << "COMMAND"
        << std::endl;
    
    return outputstream.str();

}

std::string formatSingleProcess(ProcessInfo &info, long totalMemory) 
{
    std::stringstream outputstream;
    outputstream << std::right << std::fixed << std::setfill(' ');
    outputstream << std::setw(5) << info.pid 
        << std::setw(8) << pagesToKB(info.size) 
        << std::setw(7) << pagesToKB(info.resident) 
        << std::setw(7) << pagesToKB(info.share) 
        << std::setw(2) << info.state
       << std::setw(6) << std::setprecision(1) << info.cpu_percent 
       << std::setw(6) << static_cast<double>(pagesToKB(info.resident)) / static_cast<double>(totalMemory)
       << std::setw(38) << formatComm(info.comm).c_str() << std::endl;
    return outputstream.str();
}

std::string formatProcessTable(std::vector<ProcessInfo> &info, long totalMemory, int y, sortBy sorting)
{
    std::stringstream outputstream;

    sortProcesses(info, sorting);
    for (int i = 0; i < y; i++)
        outputstream << formatSingleProcess(info[i], totalMemory);

    return outputstream.str();
}
