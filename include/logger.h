#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "console_ui.h"

class Logger {
public:
    Logger(const std::string& directory, const std::string& format);
    ~Logger();

    void log(const DisplayInfo& info);
    void setFormat(const std::string& format) { format_ = format; }

private:
    std::string directory_;
    std::string format_;
    std::string current_date_;
    std::ofstream file_;

    std::string today() const;
    void rotate();
    void writeCSV(const DisplayInfo& info);
    void writeJSON(const DisplayInfo& info);
    std::string escapeCSV(const std::string& s) const;
    static std::string defaultLogDir();
};
