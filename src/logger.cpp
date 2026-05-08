#include "../include/logger.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <shlobj.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#endif

Logger::Logger(const std::string& directory, const std::string& format)
    : directory_(directory.empty() ? defaultLogDir() : directory)
    , format_(format) {
    rotate();
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_.close();
    }
}

std::string Logger::defaultLogDir() {
#ifdef _WIN32
    char appdata[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, appdata))) {
        return std::string(appdata) + "\\ollama-monitor\\logs";
    }
    return "logs";
#else
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : ".";
    }
    return std::string(home) + "/.ollama/logs";
#endif
}

std::string Logger::today() const {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&tt);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return buf;
}

void Logger::rotate() {
    std::string date = today();
    if (date == current_date_ && file_.is_open()) {
        return;
    }

    if (file_.is_open()) {
        file_.close();
    }

    current_date_ = date;

#ifdef _WIN32
    _mkdir(directory_.c_str());
#else
    mkdir(directory_.c_str(), 0755);
#endif

    std::string ext = (format_ == "json") ? ".jsonl" : ".csv";
    std::string path = directory_ + "/ollama-monitor-" + date + ext;
    file_.open(path, std::ios::app);

    if (!file_.is_open()) {
        std::cerr << "\033[33mWarning: Could not open log file: " << path << "\033[0m\n";
    }
}

std::string Logger::escapeCSV(const std::string& s) const {
    if (s.find(',') != std::string::npos || s.find('"') != std::string::npos || s.find('\n') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : s) {
            if (c == '"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return s;
}

void Logger::log(const DisplayInfo& info) {
    rotate();

    if (!file_.is_open()) return;

    if (format_ == "json") {
        writeJSON(info);
    } else {
        writeCSV(info);
    }

    file_.flush();
}

void Logger::writeCSV(const DisplayInfo& info) {
    std::string gpu_names;
    for (size_t i = 0; i < info.gpu_infos.size(); i++) {
        if (i > 0) gpu_names += "; ";
        gpu_names += info.gpu_infos[i].name;
    }

    int running_count = 0;
    std::string running_names;
    if (info.ollama_status) {
        running_count = static_cast<int>(info.ollama_status->models.size());
        for (size_t i = 0; i < info.ollama_status->models.size(); i++) {
            if (i > 0) running_names += "; ";
            running_names += info.ollama_status->models[i].name;
        }
    }

    file_ << current_date_ << "," << escapeCSV(info.current_time) << ","
          << std::fixed << std::setprecision(1)
          << info.system_info.cpu_percent << ","
          << info.system_info.ram_percent << ","
          << std::setprecision(2)
          << info.system_info.ram_used_gb << ","
          << info.system_info.ram_total_gb << ","
          << std::setprecision(1)
          << info.system_info.disk_percent << ","
          << std::setprecision(2)
          << info.system_info.disk_used_gb << ","
          << info.system_info.disk_total_gb << ","
          << std::setprecision(1);

    if (!info.gpu_infos.empty() && info.gpu_infos[0].available) {
        file_ << info.gpu_infos[0].utilization_percent << ","
              << std::setprecision(2) << info.gpu_infos[0].used_vram_gb << ","
              << info.gpu_infos[0].total_vram_gb << ","
              << info.gpu_infos[0].temperature_c;
    } else {
        file_ << "N/A,N/A,N/A,N/A";
    }

    file_ << "," << running_count << ","
          << escapeCSV(running_names) << ","
          << escapeCSV(gpu_names)
          << "\n";
}

void Logger::writeJSON(const DisplayInfo& info) {
    nlohmann::json j;
    j["timestamp"] = info.current_time;
    j["date"] = current_date_;

    j["cpu"]["percent"] = std::round(info.system_info.cpu_percent * 10) / 10;
    j["ram"]["percent"] = std::round(info.system_info.ram_percent * 10) / 10;
    j["ram"]["used_gb"] = std::round(info.system_info.ram_used_gb * 100) / 100;
    j["ram"]["total_gb"] = std::round(info.system_info.ram_total_gb * 100) / 100;
    j["disk"]["percent"] = std::round(info.system_info.disk_percent * 10) / 10;
    j["disk"]["used_gb"] = std::round(info.system_info.disk_used_gb * 100) / 100;
    j["disk"]["total_gb"] = std::round(info.system_info.disk_total_gb * 100) / 100;

    if (!info.gpu_infos.empty() && info.gpu_infos[0].available) {
        auto& gpu = info.gpu_infos[0];
        j["gpu"]["name"] = gpu.name;
        j["gpu"]["utilization_percent"] = std::round(gpu.utilization_percent * 10) / 10;
        j["gpu"]["vram_used_gb"] = std::round(gpu.used_vram_gb * 100) / 100;
        j["gpu"]["vram_total_gb"] = std::round(gpu.total_vram_gb * 100) / 100;
        j["gpu"]["temperature_c"] = gpu.temperature_c;
    }

    if (info.ollama_status) {
        j["running_models"]["count"] = info.ollama_status->models.size();
        auto& models = j["running_models"]["models"];
        models = nlohmann::json::array();
        for (const auto& m : info.ollama_status->models) {
            models.push_back(m.name);
        }
    }

    file_ << j.dump() << "\n";
}
