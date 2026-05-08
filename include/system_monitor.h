#pragma once

#include <string>
#include <cstdint>

struct SystemInfo {
    double cpu_percent = 0.0;
    double ram_total_gb = 0.0;
    double ram_used_gb = 0.0;
    double ram_percent = 0.0;
    double disk_total_gb = 0.0;
    double disk_used_gb = 0.0;
    double disk_percent = 0.0;
    bool available = false;
};

class SystemMonitor {
public:
    SystemMonitor();
    SystemInfo getInfo();

private:
    uint64_t prev_idle_ = 0;
    uint64_t prev_total_ = 0;
    bool first_cpu_ = true;

    double calcCPU();
    SystemInfo readProcFS();
    SystemInfo readWindowsAPI();
};
