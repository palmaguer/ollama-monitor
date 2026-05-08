#pragma once

#include <string>

struct Theme {
    std::string header_fg = "37";
    std::string header_bg = "44";
    std::string header = "1;37;44";
    std::string section_system = "1;32";
    std::string section_gpu = "1;36";
    std::string section_models = "1;35";
    std::string section_available = "1;34";
    std::string error = "31";
    std::string dim = "90";
    std::string paused = "1;33";
    std::string color_good = "32";
    std::string color_warn = "33";
    std::string color_danger = "31";
    std::string model_name = "32";
    std::string selected = "4";
    std::string reset = "0";
    std::string bar_good = "32";
    std::string bar_warn = "33";
    std::string bar_danger = "31";
    bool is_light = false;

    static Theme dark() { return Theme{}; }

    static Theme light() {
        Theme t;
        t.header_fg = "30";
        t.header_bg = "106";
        t.header = "1;30;106";
        t.section_system = "1;32";
        t.section_gpu = "1;36";
        t.section_models = "1;35";
        t.section_available = "1;34";
        t.error = "31";
        t.dim = "90";
        t.paused = "1;33";
        t.color_good = "32";
        t.color_warn = "33";
        t.color_danger = "31";
        t.model_name = "32";
        t.bar_good = "32";
        t.bar_warn = "33";
        t.bar_danger = "31";
        t.is_light = true;
        return t;
    }

    std::string ansi(const std::string& code) const {
        return "\033[" + code + "m";
    }
};
