#include "../include/config_manager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <shlobj.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif

ConfigManager::ConfigManager()
    : path_(defaultConfigPath()) {
}

ConfigManager::ConfigManager(const std::string& path)
    : path_(path) {
}

std::string ConfigManager::defaultConfigPath() {
#ifdef _WIN32
    char appdata[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, appdata))) {
        return std::string(appdata) + "\\ollama-monitor\\config.json";
    }
    return "config.json";
#else
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        home = pw ? pw->pw_dir : ".";
    }
    return std::string(home) + "/.config/ollama-monitor/config.json";
#endif
}

bool ConfigManager::exists() const {
    std::ifstream f(path_);
    return f.good();
}

void ConfigManager::ensureDirectoryExists() const {
    auto dir = path_.substr(0, path_.find_last_of("/\\"));
    if (dir.empty()) return;

#ifdef _WIN32
    std::string cmd = "if not exist \"" + dir + "\" mkdir \"" + dir + "\"";
    if (system(cmd.c_str()) != 0) {}
#else
    struct stat st;
    if (stat(dir.c_str(), &st) != 0) {
        std::string cmd = "mkdir -p \"" + dir + "\"";
        if (system(cmd.c_str()) != 0) {}
    }
#endif
}

AppConfig ConfigManager::load() {
    if (!exists()) {
        config_ = AppConfig::defaults();
        return config_;
    }

    try {
        std::ifstream f(path_);
        nlohmann::json j;
        f >> j;

        if (j.contains("refresh_rate")) config_.refresh_rate = j["refresh_rate"];
        if (j.contains("ollama_url")) config_.ollama_url = j["ollama_url"];
        if (j.contains("theme")) config_.theme = j["theme"];
        if (j.contains("sparkline_length")) config_.sparkline_length = j["sparkline_length"];

        if (j.contains("logging")) {
            auto& log = j["logging"];
            if (log.contains("enabled")) config_.logging_enabled = log["enabled"];
            if (log.contains("format")) config_.log_format = log["format"];
            if (log.contains("directory")) config_.log_directory = log["directory"];
        }
    } catch (const std::exception& e) {
        std::cerr << "\033[33mWarning: Failed to parse config file: " << e.what() << "\033[0m\n";
        config_ = AppConfig::defaults();
    }

    return config_;
}

void ConfigManager::save(const AppConfig& config) {
    ensureDirectoryExists();

    nlohmann::json j;
    j["refresh_rate"] = config.refresh_rate;
    j["ollama_url"] = config.ollama_url;
    j["theme"] = config.theme;
    j["sparkline_length"] = config.sparkline_length;
    j["logging"]["enabled"] = config.logging_enabled;
    j["logging"]["format"] = config.log_format;
    j["logging"]["directory"] = config.log_directory;

    std::ofstream f(path_);
    f << j.dump(4) << std::endl;
}
