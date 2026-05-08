#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct AppConfig {
    int refresh_rate = 1;
    std::string ollama_url = "http://localhost:11434";
    std::string theme = "dark";
    bool logging_enabled = false;
    std::string log_format = "csv";

    static AppConfig defaults() { return AppConfig{}; }
};

class ConfigManager {
public:
    ConfigManager();
    explicit ConfigManager(const std::string& path);

    AppConfig load();
    void save(const AppConfig& config);
    std::string path() const { return path_; }
    bool exists() const;

    static std::string defaultConfigPath();

private:
    std::string path_;
    AppConfig config_;
    bool file_exists_ = false;

    void ensureDirectoryExists() const;
};
