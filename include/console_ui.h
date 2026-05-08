#pragma once

#include <string>
#include <vector>
#include "ollama_client.h"
#include "gpu_monitor.h"
#include "system_monitor.h"

struct DisplayInfo {
    std::vector<GPUInfo> gpu_infos;
    std::unique_ptr<OllamaStatus> ollama_status;
    std::vector<OllamaModel> available_models;
    SystemInfo system_info;
    std::string current_time;
};

class ConsoleUI {
public:
    ConsoleUI();
    ~ConsoleUI();
    
    void moveCursorHome();
    void clearToEndOfScreen();
    void clearLine();
    void display(const DisplayInfo& info);
    void refreshRate(int seconds) { refresh_rate_ = seconds; }
    void setNoClear(bool no_clear) { no_clear_ = no_clear; }
    void setPaused(bool paused) { paused_ = paused; }
    bool isPaused() const { return paused_; }

private:
    int refresh_rate_;
    bool no_clear_ = false;
    bool paused_ = false;
    
    // Helper methods for formatting
    std::string formatBytes(int64_t bytes) const;
    std::string formatTimeUntil(const std::string& expires_at) const;
    std::string truncateString(const std::string& str, size_t max_length) const;
    std::string getCurrentTime() const;
    std::string getProgressBar(double percentage, int width = 20) const;
    
    void displayGPUInfo(const std::vector<GPUInfo>& gpu_infos);
    void displayOllamaInfo(const std::unique_ptr<OllamaStatus>& status);
    void displayRunningModels(const std::vector<OllamaRunningModel>& models);
    void displayAvailableModels(const std::vector<OllamaModel>& models);
    void displaySystemStats(const SystemInfo& info);
};