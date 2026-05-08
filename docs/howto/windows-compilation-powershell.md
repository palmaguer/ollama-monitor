# Windows Compilation using Powershell (MSVC + CMake)

## 1. Install dependencies from PowerShell (Admin)

Run these commands in an elevated PowerShell window:

```powershell
# Visual Studio Build Tools + required C++ components
winget install --id Microsoft.VisualStudio.2022.BuildTools -e --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --includeRecommended"

# CMake
winget install --id Kitware.CMake -e

# Optional: Ninja generator
winget install --id Ninja-build.Ninja -e
```

> Reference Screenshot
>
>![Screenshot](/docs/img/screenshot-visualstudio-installer.png)


## 2. Open a PowerShell session with MSVC tools loaded

Use `vswhere` to locate the install path and load the VS developer shell:

```powershell
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath

# Print the path to confirm:
$vsPath

# Load ps1 file
& "$vsPath\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64
```

If `Launch-VsDevShell.ps1` is missing or fails, review [Troubleshooting](#troubleshooting) or use fallback:

```powershell
cmd /k """$vsPath\Common7\Tools\VsDevCmd.bat"" -arch=amd64 -host_arch=amd64"
```


## 3. Verify toolchain

```powershell
where.exe cl
where.exe nmake
cmake --version
```

> If `where.exe nmake` returns nothing, your VS C++ components are not installed correctly.


## 4. Configure and Build with NMake

From the project root:

```powershell
cmake -S . -B build -G "NMake Makefiles"
cmake --build build
```


## Optional: Build with Ninja instead of NMake

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

> **Note:** Ninja still needs MSVC (`cl.exe`) unless you are using a different compiler toolchain.


## Troubleshooting

- Running scripts is disabled on this system:

  ```powershell
  Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
  ```
