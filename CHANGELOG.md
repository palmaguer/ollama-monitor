# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **Config File Support** - `~/.config/ollama-monitor/config.json` for persistent settings (refresh rate, URL, theme, logging). CLI flags override config values.
- **Pause/Resume** - Press SPACE or P to freeze/resume the display
- **Model Details** - Context size and architecture shown for each running model (from `/api/show`)
- **System Stats** - CPU, RAM, and disk usage monitoring via `/proc`
- **Themes** - Dark/light theme toggle with `T` key, configured via `theme` field in config
- **Logging** - Optional CSV/JSON metrics logging to `~/.ollama/logs/` with daily rotation
- **GPU Sparklines** - Inline Unicode sparklines (▁▂▃▄▅▆▇█) showing utilization and VRAM history per GPU
- **Temperature Alerts** - Visual warnings at configurable thresholds (yellow at 75°C, red blink at 85°C)

## [v1.1.1] - 2026-05-07

### Changed
- **UTF-8 Progress Bars** - Replaced `|` pipe characters with Unicode block chars (`█`/`░`) for cleaner output
- **Documentation Enhancement** - Updated documentation with detailed compilation guides. 

## [v1.1.0] - 2026-05-07

### Added
- **Linux/WSL2 Support** - Full support for Ubuntu and WSL2
- **GPU Monitoring on Linux** - NVML support via `libnvidia-ml.so`
- **HTTP Client on Linux** - libcurl implementation (WinHTTP preserved for Windows)
- **Cross-platform Time Functions** - Use `timegm()` on Linux, `_mkgmtime()` on Windows
- **GitHub Actions CI/CD** - Automatic builds for Linux and Windows

### Changed
- Updated CMakeLists.txt with conditional Linux/Windows build configuration
- Updated README with Linux/WSL2 build instructions and requirements

### Tested
- GPU: NVIDIA GeForce RTX 4080 SUPER
  - VRAM monitoring: working
  - GPU utilization: working
  - Temperature: working
  - Power consumption: working
- Ollama: 5 models detected via API

## [v1.0.0] - (Original Release)

### Added
- GPU Monitoring (NVIDIA full support via NVML, AMD/Intel basic via DXGI)
- Ollama Integration (running models, available models)
- Top-style console UI with auto-refresh
- Command-line options (refresh rate, URL, once mode)
- Windows WinHTTP HTTP client