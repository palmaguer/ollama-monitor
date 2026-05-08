#include "../include/metrics_history.h"
#include <algorithm>
#include <cmath>

static const char* SPARK_CHARS = "\xe2\x96\x81\xe2\x96\x82\xe2\x96\x83\xe2\x96\x84\xe2\x96\x85\xe2\x96\x86\xe2\x96\x87\xe2\x96\x88";

GpuHistory::GpuHistory(size_t max_points)
    : max_points_(max_points > 0 ? max_points : 1) {
}

void GpuHistory::push(double util_pct, double vram_pct) {
    util_history_.push_back(util_pct);
    vram_history_.push_back(vram_pct);
    while (util_history_.size() > max_points_) {
        util_history_.pop_front();
    }
    while (vram_history_.size() > max_points_) {
        vram_history_.pop_front();
    }
}

std::string GpuHistory::getUtilSparkline(int width) const {
    return render(util_history_, width);
}

std::string GpuHistory::getVRAMSparkline(int width) const {
    return render(vram_history_, width);
}

std::string GpuHistory::render(const std::deque<double>& data, int width) {
    if (data.empty() || width < 1) return "";

    std::string result;
    size_t n = data.size();

    auto minmax = std::minmax_element(data.begin(), data.end());
    double min_val = *minmax.first;
    double max_val = *minmax.second;
    double range = max_val - min_val;
    if (range < 0.1) range = 100.0;

    for (int i = 0; i < width; i++) {
        size_t idx = (n > 1) ? static_cast<size_t>(static_cast<double>(i) * (n - 1) / (width - 1)) : 0;
        if (idx >= n) idx = n - 1;

        double normalized = (data[idx] - min_val) / range;
        if (normalized < 0.0) normalized = 0.0;
        if (normalized > 1.0) normalized = 1.0;

        int spark_idx = static_cast<int>(std::round(normalized * 7));
        if (spark_idx < 0) spark_idx = 0;
        if (spark_idx > 7) spark_idx = 7;

        result += &SPARK_CHARS[spark_idx * 3];
    }

    return result;
}
