# Linux / WSL2 Compilation (GCC + CMake)

## 1. Install dependencies

Run these commands in your Linux shell (Ubuntu / WSL2):

```bash
sudo apt update
sudo apt install -y build-essential cmake libcurl4-openssl-dev
```


## 2. Verify toolchain

```bash
gcc --version
g++ --version
cmake --version
```

> If `cmake` or `g++` is not found, re-run step 1 and confirm package installation completed successfully.


## 3. Get the source code

```bash
git clone https://github.com/palmaguer/ollama-monitor.git
cd ollama-monitor
```


## 4. Configure and Build

From the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```


## 5. Run the binary

```bash
./build/ollama-monitor
```

> Output: `build/ollama-monitor`


## Troubleshooting

- Missing `libcurl` at configure/build time:

  ```bash
  sudo apt install -y libcurl4-openssl-dev
  ```

- Permission denied when running binary:

  ```bash
  chmod +x ./build/ollama-monitor
  ```
