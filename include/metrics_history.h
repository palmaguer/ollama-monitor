#pragma once

#include <deque>
#include <string>
#include <cstddef>

class GpuHistory {
public:
    explicit GpuHistory(size_t max_points = 20);

    void push(double util_pct, double vram_pct);
    std::string getUtilSparkline(int width = 10) const;
    std::string getVRAMSparkline(int width = 10) const;
    void setMaxPoints(size_t n) { max_points_ = n; }
    size_t maxPoints() const { return max_points_; }

private:
    size_t max_points_;
    std::deque<double> util_history_;
    std::deque<double> vram_history_;

    static std::string render(const std::deque<double>& data, int width);
};
