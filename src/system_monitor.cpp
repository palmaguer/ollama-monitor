#include "system_monitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/statvfs.h>

SystemMonitor::SystemMonitor() {
    prev_idle_ = 0;
    prev_total_ = 0;
}

double SystemMonitor::calcCPU() {
    std::ifstream stat("/proc/stat");
    std::string line;
    if (!std::getline(stat, line)) return 0.0;

    std::istringstream ss(line);
    std::string cpu;
    uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    uint64_t idle_time = idle + iowait;
    uint64_t total_time = user + nice + system + idle + iowait + irq + softirq + steal;

    if (first_cpu_) {
        prev_idle_ = idle_time;
        prev_total_ = total_time;
        first_cpu_ = false;
        return 0.0;
    }

    uint64_t delta_idle = idle_time - prev_idle_;
    uint64_t delta_total = total_time - prev_total_;

    prev_idle_ = idle_time;
    prev_total_ = total_time;

    if (delta_total == 0) return 0.0;
    return 100.0 * (1.0 - (double)delta_idle / (double)delta_total);
}

SystemInfo SystemMonitor::readProcFS() {
    SystemInfo info;
    info.cpu_percent = calcCPU();

    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    uint64_t mem_total_kb = 0, mem_avail_kb = 0;

    std::string token;
    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        if (line.rfind("MemTotal:", 0) == 0) {
            iss >> token >> mem_total_kb;
        } else if (line.rfind("MemAvailable:", 0) == 0) {
            iss >> token >> mem_avail_kb;
        }
    }

    if (mem_total_kb > 0) {
        info.ram_total_gb = mem_total_kb / (1024.0 * 1024.0);
        double mem_used_kb = mem_total_kb - mem_avail_kb;
        info.ram_used_gb = mem_used_kb / (1024.0 * 1024.0);
        info.ram_percent = 100.0 * mem_used_kb / mem_total_kb;
    }

    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        uint64_t total = (uint64_t)vfs.f_frsize * (uint64_t)vfs.f_blocks;
        uint64_t free = (uint64_t)vfs.f_frsize * (uint64_t)vfs.f_bavail;
        info.disk_total_gb = total / (1024.0 * 1024.0 * 1024.0);
        info.disk_used_gb = (total - free) / (1024.0 * 1024.0 * 1024.0);
        info.disk_percent = total > 0 ? 100.0 * (total - free) / total : 0.0;
    }

    info.available = true;
    return info;
}

SystemInfo SystemMonitor::readWindowsAPI() {
    SystemInfo info;
    info.available = false;
    return info;
}

SystemInfo SystemMonitor::getInfo() {
#ifdef _WIN32
    return readWindowsAPI();
#else
    return readProcFS();
#endif
}
