# Roadmap

## v1.1.0 — Released
- Linux/WSL2 cross-platform support
- GPU monitoring via `libnvidia-ml.so` on Linux
- Multi-GPU display
- GitHub Actions CI/CD
- Screen flicker fix
- UTF-8 block progress bars

## v1.2.0 — In Progress

### Done
- **Config file** — `~/.config/ollama-monitor/config.json` for persistent settings (refresh rate, URL, theme, logging)
- **Pause/Resume** — toggle live updates with SPACE key
- **Model details** — context size and architecture from `/api/show`
- **System stats** — CPU, RAM, disk usage via `/proc` (Linux)
- **Themes** — dark/light toggle via `T` key, configurable in config file
- **Logging** — optional CSV/JSON metrics dump to `~/.ollama/logs/` with daily rotation

### Must-have

### Should-have

### Nice-to-have
- **GPU history sparklines** — inline terminal bar charts over time
- **Temperature alerts** — visual warning when thresholds exceeded

## Future

- **Prometheus exporter** — `/metrics` endpoint for Grafana integration
- **ncurses TUI** — full interactive interface (htop-like)
- **Multi-server** — monitor several Ollama hosts from one window
- **Dashboard layout** — reorder/hide sections via config
