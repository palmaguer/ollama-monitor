#include "../include/console_ui.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

ConsoleUI::ConsoleUI() : refresh_rate_(1), no_clear_(false) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleTitleA("Ollama Monitor");
#endif
}

ConsoleUI::~ConsoleUI() {
}

void ConsoleUI::moveCursorHome() {
    std::cout << "\033[H";
    std::cout.flush();
}

void ConsoleUI::clearToEndOfScreen() {
    std::cout << "\033[J";
    std::cout.flush();
}

void ConsoleUI::clearLine() {
    std::cout << "\033[K";
}

std::string ConsoleUI::formatBytes(int64_t bytes) const {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unit_index < 4) {
        size /= 1024.0;
        unit_index++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unit_index];
    return oss.str();
}

std::string ConsoleUI::formatTimeUntil(const std::string& expires_at) const {
    if (expires_at.empty()) {
        return "N/A";
    }

    std::tm tm = {};
    int year, month, day, hour, min, sec;

    if (sscanf(expires_at.c_str(), "%d-%d-%dT%d:%d:%d",
               &year, &month, &day, &hour, &min, &sec) == 6) {
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = min;
        tm.tm_sec = sec;

#ifdef _WIN32
        time_t expires_time = _mkgmtime(&tm);
#else
        time_t expires_time = timegm(&tm);
#endif
        time_t now = time(nullptr);

        std::tm* now_tm = gmtime(&now);
#ifdef _WIN32
        time_t now_utc = _mkgmtime(now_tm);
#else
        time_t now_utc = timegm(now_tm);
#endif

        double diff_seconds = difftime(expires_time, now_utc);

        if (diff_seconds <= 0) {
            return "Expired";
        }

        int minutes = static_cast<int>(diff_seconds / 60);
        int seconds = static_cast<int>(diff_seconds) % 60;

        std::ostringstream oss;
        if (minutes > 0) {
            oss << minutes << "m " << seconds << "s";
        } else {
            oss << seconds << "s";
        }
        return oss.str();
    }

    return expires_at;
}

std::string ConsoleUI::truncateString(const std::string& str, size_t max_length) const {
    if (str.length() <= max_length) {
        return str;
    }
    return str.substr(0, max_length - 3) + "...";
}

std::string ConsoleUI::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string ConsoleUI::getProgressBar(double percentage, int width) const {
    int filled = static_cast<int>((percentage / 100.0) * width);
    if (filled > width) filled = width;
    if (filled < 0) filled = 0;

    std::string bar;
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            bar += "\xe2\x96\x88";
        } else {
            bar += "\xe2\x96\x91";
        }
    }
    return bar;
}

static const std::string& barColor(const Theme& theme, double pct) {
    if (pct > 90) return theme.bar_danger;
    if (pct > 70) return theme.bar_warn;
    return theme.bar_good;
}

void ConsoleUI::displayGPUInfo(const std::vector<GPUInfo>& gpu_infos, const std::vector<GpuHistory>& gpu_history) {
    std::cout << theme_.ansi(theme_.section_gpu);
    std::cout << "=== GPU Status ===" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    if (gpu_infos.empty()) {
        std::cout << theme_.ansi(theme_.color_warn) << "  GPU monitoring unavailable (NVML not found)" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        return;
    }

    for (size_t idx = 0; idx < gpu_infos.size(); idx++) {
        const auto& gpu_info = gpu_infos[idx];

        if (!gpu_info.available) {
            continue;
        }

        if (gpu_infos.size() > 1) {
            std::cout << "  \033[1mGPU " << gpu_info.index << ":\033[0m " << gpu_info.name;
        } else {
            std::cout << "  \033[1mGPU:\033[0m " << gpu_info.name;
        }
        clearLine();
        std::cout << "\n";

        double vram_percent = gpu_info.getVRAMUsagePercent();
        std::cout << "  \033[1mVRAM:\033[0m ";
        std::cout << theme_.ansi(barColor(theme_, vram_percent));
        std::cout << getProgressBar(vram_percent, 30) << " ";
        std::cout << std::fixed << std::setprecision(1) << vram_percent << "% ";
        std::cout << "(" << std::setprecision(2) << gpu_info.used_vram_gb << "/"
                  << gpu_info.total_vram_gb << " GB)" << theme_.ansi(theme_.reset);
        if (idx < gpu_history.size()) {
            std::cout << " " << gpu_history[idx].getVRAMSparkline(12);
        }
        clearLine();
        std::cout << "\n";

        std::cout << "  \033[1mUtil:\033[0m ";
        std::cout << theme_.ansi(barColor(theme_, gpu_info.utilization_percent));
        std::cout << getProgressBar(gpu_info.utilization_percent, 30) << " "
                  << std::fixed << std::setprecision(0) << gpu_info.utilization_percent << "%" << theme_.ansi(theme_.reset);
        if (idx < gpu_history.size()) {
            std::cout << " " << gpu_history[idx].getUtilSparkline(12);
        }
        clearLine();
        std::cout << "\n";

        std::cout << "  \033[1mTemp:\033[0m ";
        if (gpu_info.temperature_c >= alert_temp_critical_) {
            std::cout << "\033[5;1;31m";  // blink + bold + red
            std::cout << "\xe2\x9a\xa0 ";  // ⚠
            std::cout << gpu_info.temperature_c << " C (CRITICAL)" << theme_.ansi(theme_.reset);
        } else if (gpu_info.temperature_c >= alert_temp_warning_) {
            std::cout << "\033[1;33m";  // bold + yellow
            std::cout << gpu_info.temperature_c << " C (WARNING)" << theme_.ansi(theme_.reset);
        } else {
            std::cout << theme_.ansi(theme_.color_good) << gpu_info.temperature_c << " C" << theme_.ansi(theme_.reset);
        }
        std::cout << "  ";
        std::cout << "\033[1mPower:\033[0m " << gpu_info.power_watts << " W";
        clearLine();
        std::cout << "\n";

        if (gpu_info.temperature_c >= alert_temp_critical_) {
            std::cout << "    \033[5;1;31m\xe2\x9a\xa0 TEMP ALERT: " << gpu_info.temperature_c
                      << " C exceeds critical threshold ("
                      << alert_temp_critical_ << " C)\033[0m";
            clearLine();
            std::cout << "\n";
        }

        if (gpu_infos.size() > 1 && idx < gpu_infos.size() - 1) {
            clearLine();
            std::cout << "\n";
        }
    }
}

void ConsoleUI::displayRunningModels(const std::vector<OllamaRunningModel>& models) {
    clearLine();
    std::cout << "\n" << theme_.ansi(theme_.section_models);
    std::cout << "=== Running Models ===" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    if (models.empty()) {
        std::cout << theme_.ansi(theme_.color_warn) << "  No models currently loaded" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        return;
    }

    std::cout << "  " << theme_.ansi(theme_.selected) << std::left
              << std::setw(30) << "MODEL"
              << std::setw(12) << "SIZE"
              << std::setw(12) << "PARAMS"
              << std::setw(10) << "QUANT"
              << std::setw(10) << "CTX"
              << std::setw(12) << "EXPIRES"
              << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    for (const auto& model : models) {
        std::string ctx_str;
        if (model.model_detail.context_length > 0) {
            ctx_str = std::to_string(model.model_detail.context_length / 1024) + "K";
        } else {
            ctx_str = "-";
        }

        std::cout << "  " << theme_.ansi(theme_.model_name) << std::left
                  << std::setw(30) << truncateString(model.name, 29)
                  << theme_.ansi(theme_.reset)
                  << std::setw(12) << formatBytes(model.size)
                  << std::setw(12) << model.details.parameter_size
                  << std::setw(10) << model.details.quantization_level
                  << std::setw(10) << ctx_str
                  << std::setw(12) << formatTimeUntil(model.expires_at);
        clearLine();
        std::cout << "\n";
    }
}

void ConsoleUI::displayAvailableModels(const std::vector<OllamaModel>& models) {
    clearLine();
    std::cout << "\n" << theme_.ansi(theme_.section_available);
    std::cout << "=== Available Models (" << models.size() << ") ===" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    if (models.empty()) {
        std::cout << theme_.ansi(theme_.color_warn) << "  No models installed" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        return;
    }

    size_t display_count = models.size() < 10 ? models.size() : 10;

    std::cout << "  " << theme_.ansi(theme_.selected) << std::left
              << std::setw(35) << "MODEL"
              << std::setw(12) << "SIZE"
              << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    for (size_t i = 0; i < display_count; i++) {
        const auto& model = models[i];
        std::cout << "  " << std::left
                  << std::setw(35) << truncateString(model.name, 34)
                  << std::setw(12) << formatBytes(model.size);
        clearLine();
        std::cout << "\n";
    }

    if (models.size() > 10) {
        std::cout << "  " << theme_.ansi(theme_.dim) << "... and " << (models.size() - 10) << " more" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
    }
}

void ConsoleUI::displayOllamaInfo(const std::unique_ptr<OllamaStatus>& status) {
    if (!status) {
        clearLine();
        std::cout << "\n" << theme_.ansi(theme_.error);
        std::cout << "=== Ollama Status ===" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        std::cout << "  " << theme_.ansi(theme_.color_danger) << "Cannot connect to Ollama server" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        std::cout << "  " << theme_.ansi(theme_.dim) << "Make sure Ollama is running (ollama serve)" << theme_.ansi(theme_.reset);
        clearLine();
        std::cout << "\n";
        return;
    }

    displayRunningModels(status->models);
}

void ConsoleUI::displaySystemStats(const SystemInfo& info) {
    if (!info.available) return;

    clearLine();
    std::cout << "\n" << theme_.ansi(theme_.section_system);
    std::cout << "=== System ===" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    std::cout << "  \033[1mCPU:\033[0m ";
    std::cout << theme_.ansi(barColor(theme_, info.cpu_percent));
    std::cout << getProgressBar(info.cpu_percent, 20) << " "
              << std::fixed << std::setprecision(1) << info.cpu_percent << "%" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    std::cout << "  \033[1mRAM:\033[0m ";
    std::cout << theme_.ansi(barColor(theme_, info.ram_percent));
    std::cout << getProgressBar(info.ram_percent, 20) << " "
              << std::fixed << std::setprecision(1) << info.ram_percent << "% "
              << "(" << std::setprecision(2) << info.ram_used_gb << "/"
              << std::setprecision(2) << info.ram_total_gb << " GB)" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";

    std::cout << "  \033[1mDisk:\033[0m ";
    std::cout << theme_.ansi(barColor(theme_, info.disk_percent));
    std::cout << getProgressBar(info.disk_percent, 20) << " "
              << std::fixed << std::setprecision(1) << info.disk_percent << "% "
              << "(" << std::setprecision(2) << info.disk_used_gb << "/"
              << std::setprecision(2) << info.disk_total_gb << " GB)" << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";
}

void ConsoleUI::display(const DisplayInfo& info) {
    if (!no_clear_) {
        moveCursorHome();
    }

    std::cout << theme_.ansi(theme_.header);
    std::cout << " OLLAMA MONITOR                                              ";
    std::cout << getCurrentTime() << " " << theme_.ansi(theme_.reset);
    clearLine();
    std::cout << "\n";
    clearLine();
    std::cout << "\n";

    displaySystemStats(info.system_info);

    displayGPUInfo(info.gpu_infos, info.gpu_history);

    displayOllamaInfo(info.ollama_status);

    displayAvailableModels(info.available_models);

    clearLine();
    std::cout << "\n";
    if (paused_) {
        std::cout << theme_.ansi(theme_.paused) << "  *** PAUSED *** Press SPACE to resume  " << theme_.ansi(theme_.reset);
    } else {
        std::cout << theme_.ansi(theme_.dim) << "Press SPACE to pause | T to toggle theme | Ctrl+C to exit | Refreshing every "
                  << refresh_rate_ << "s" << theme_.ansi(theme_.reset);
    }
    clearLine();
    std::cout << "\n";

    clearToEndOfScreen();
}
