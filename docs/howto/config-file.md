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
        "format": "csv"
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

## Precedence

CLI flags > Config file > Hardcoded defaults

```bash
# Config file sets refresh_rate=2, but CLI overrides to 5
ollama-monitor -r 5
```
