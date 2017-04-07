#include "processInfo.h"
#include "cpuInfo.h"
#include "../utility/readFile.h"
#include "dirent.h"
#include <iostream>

const char *basedir = "/proc";

std::string appendPID(int pid, const char *basedir)
{
    std::string dir = basedir;
    dir += "/";
    dir += std::to_string(pid);
    dir += "/";
    return dir;
}

void readStatus(std::string &file, ProcessInfo &info) 
{
    std::stringstream input;
    readFile(file, input);

    std::string line;
    while (!input.eof()) 
    {
        std::getline(input, line);

        if (line.find("Tgid") != std::string::npos) 
        {
            line = line.substr(5);
            info.tgid = std::stoi(line);
            return;
        }
    }
}

void readStatm(std::string &file, ProcessInfo &info) 
{
    std::stringstream input;
    readFile(file, input);

    input >> info.size;
    input >> info.resident;
    input >> info.share;
    input >> info.trs;
    input >> info.lrs;
    input >> info.drs;
    input >> info.dt;
}

void readStat(std::string &file, ProcessInfo &info) 
{
    std::stringstream input;
    readFile(file, input);

    input >> info.pid;
    input >> info.comm;
    input >> info.state;
    input >> info.ppid;
    input >> info.pgrp;
    input >> info.session;
    input >> info.tty_nr;
    input >> info.tpgid;
    input >> info.flags;
    input >> info.minflt;
    input >> info.cminflt;
    input >> info.majflt;
    input >> info.cmajflt;
    input >> info.utime;
    input >> info.stime;
    input >> info.cutime;
    input >> info.cstime;
    input >> info.priority;
    input >> info.nice;
    input >> info.num_threads;
    input >> info.itrealvalue;
    input >> info.starttime;
    input >> info.vsize;
    input >> info.rss;
    input >> info.rsslim;
    input >> info.startcode;
    input >> info.endcode;
    input >> info.startstack;
    input >> info.kstkesp;
    input >> info.kstkeip;
    input >> info.signal;
    input >> info.blocked;
    input >> info.sigignore;
    input >> info.sigcatch;
    input >> info.wchan;
    input >> info.nswap;
    input >> info.cnswap;
    input >> info.exit_signal;
    input >> info.processor;
    input >> info.rt_priority;
    input >> info.policy;
    input >> info.delayacct_blkio_ticks;
    input >> info.guest_time;
    input >> info.cguest_time;
}

void readCommandLine(std::string pdir, ProcessInfo &info) 
{
    std::stringstream input;
    readFile(pdir + "cmdline", input);

    std::string cmd;
    std::getline(input, cmd);

    if (cmd.size() != 0) 
    {
        for (char &c: cmd)
            if (c == 0)
                c = ' ';

        if (cmd[0] == ' ')
            cmd = cmd.substr(1);
        if (cmd[cmd.size() - 1] == ' ')
            cmd = cmd.substr(0, cmd.size() - 1);
    }
    else 
    {
        std::stringstream input2;
        readFile(pdir + "comm", input2);

        std::getline(input2, cmd);
    }

    info.command_line = cmd;
}

ProcessInfo getProcess(int pid, const char *basedir)
{
    std::string pidDir = appendPID(pid, basedir);
    std::string stat = pidDir + "stat";
    std::string statm = pidDir + "statm";
    std::string status = pidDir + "status";
    std::string threads = pidDir + "tasks";

    ProcessInfo info;

    readStat(stat, info);
    readStatm(statm, info);
    readStatus(status, info);
    readCommandLine(pidDir, info);

    DIR *pdir = NULL;
    if ((pdir = opendir(threads.c_str())) != NULL) 
    {
        closedir(pdir);
        info.threads = getAllProcesses(threads.c_str());
        info.num_threads = info.threads.size();
    }
    return info;
}

std::vector<ProcessInfo> getAllProcesses(const char *basedir) 
{
    std::vector<ProcessInfo> result;

    DIR *pDir = NULL;
    struct dirent *pent = NULL;

    pDir = opendir(basedir);
    if (pDir == NULL) 
    {
        std::cerr << "Error opening " << basedir << std::endl;
        throw std::exception();
    }

    std::string pidString;
    while ((pent = readdir(pDir)) != NULL) 
    {
        if (pent == NULL) 
        {
            std::cerr << "ERROR! pent could not be initialised correctly" << std::endl;
            throw std::exception();
        }
        pidString = pent->d_name;

        if (pidString[0] < '1' || pidString[0] > '9')
            continue;
        try
        {
            result.push_back(getProcess(std::stoi(pidString), basedir));
        }
        catch(std::exception &e)
        {
            std::cerr << "Process " << pidString << " is not available" << std::endl;
        }
    }

    closedir(pDir);

    return result;
}

std::vector<ProcessInfo> getProcessesInfo()
{
    static long long lastCPUTotalTime = 0;
    static std::vector<ProcessInfo> lastProc = getAllProcesses(basedir);
    std::vector<ProcessInfo> result = getAllProcesses(basedir);

    CpuInfo cpu = getCPUInfo();

    size_t changedSize = (lastProc.size() > result.size()) ? result.size() : lastProc.size();

    long long currCPUTotalTime = cpu.total_time();
    long long diffCPUTotalTime = currCPUTotalTime - lastCPUTotalTime;

    for (size_t i = 0; i < changedSize; ++i) 
    {
        ProcessInfo *last = &lastProc[i];
        ProcessInfo *curr = &result[i];

        long long lastTotalTime = last->utime + last->stime + last->cutime + last->cstime;
        long long currTotalTime = curr->utime + curr->stime + curr->cstime + curr->cutime;
        long long diffTotalTime = currTotalTime - lastTotalTime;

        curr->cpu_percent = (double) diffTotalTime / diffCPUTotalTime * 100;
    }

    lastProc = result;
    lastCPUTotalTime = currCPUTotalTime;

    return result;
}