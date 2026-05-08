# Config File

Ollama Monitor supports persistent configuration via a JSON file. CLI flags always take precedence over config file values.

## Location

| Platform | Path |
|----------|------|
| Linux | `~/.config/ollama-monitor/config.json` |
| Windows | `%APPDATA%\ollama-monitor\config.json` |

You can specify a custom path with `-c` / `--config`:

```bash
ollama-monitor -c /path/to/config.json
```

## Example

```json
{
    "refresh_rate": 2,
    "ollama_url": "http://localhost:11434",
    "theme": "dark",
    "logging": {
        "enabled": false,
        "format": "csv",
        "directory": ""
    }
}
```

## Fields

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `refresh_rate` | int | `1` | Refresh interval in seconds (min 1) |
| `ollama_url` | string | `http://localhost:11434` | Ollama server URL |
| `theme` | string | `"dark"` | Color theme (`"dark"` or `"light"`) |
| `logging.enabled` | bool | `false` | Enable metrics logging to file |
| `logging.format` | string | `"csv"` | Log format (`"csv"` or `"json"`) |
| `logging.directory` | string | `""` | Log directory (empty = `~/.ollama/logs/` on Linux, `%APPDATA%\ollama-monitor\logs` on Windows) |

## Features Added in v1.2.0

### System Stats
CPU, RAM, and disk usage displayed as a new System section with progress bars, above GPU information.

### Themes
Press `T` to toggle between dark (default) and light themes. Set `"theme": "light"` in config for persistent selection.

### Logging
When enabled, writes a metrics snapshot each refresh cycle to a dated file:
- CSV: `ollama-monitor-YYYY-MM-DD.csv`
- JSONL: `ollama-monitor-YYYY-MM-DD.jsonl`

Fields logged: timestamp, CPU %, RAM %, disk %, GPU utilization, VRAM usage, running model names.

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `SPACE` or `P` | Pause/Resume live updates |
| `T` | Toggle dark/light theme |
| `Ctrl+C` | Exit |

## Precedence

CLI flags > Config file > Hardcoded defaults

```bash
# Config file sets refresh_rate=2, but CLI overrides to 5
ollama-monitor -r 5
```
