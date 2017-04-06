#pragma once

struct CpuInfo 
{
    unsigned long long user_time;
    unsigned long long nice_time;
    unsigned long long system_time;
    unsigned long long idle_time;
    unsigned long long io_wait_time;
    unsigned long long irq_time;
    unsigned long long softirq_time;
    unsigned long long steal_time;
    unsigned long long guest_time;
    unsigned long long guest_nice_time;

    unsigned long long total_idle_time() const {
        return idle_time + io_wait_time;
    }

    unsigned long long total_system_time() const {
        return system_time + irq_time + softirq_time;
    }

    unsigned long long total_user_time() const {
        return user_time + nice_time;
    }

    unsigned long long total_virtual_time() const {
        return steal_time + guest_time + guest_nice_time;
    }

    unsigned long long total_time() const {
        return total_system_time()
               + total_idle_time()
               + total_virtual_time()
               + user_time
               + nice_time;
    }
};


CpuInfo getCPUInfo();