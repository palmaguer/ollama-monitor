# Contributing

Thank you for considering contributing to this project! Whether you're reporting a bug, suggesting a 
feature, or submitting a pull request, your contributions help improve the tool for everyone. This guide 
outlines how to get started and what to expect when contributing.

This project is a fork of [yonie/ollama-monitor](https://github.com/yonie/ollama-monitor), with continued 
development and support during my free time. As a newcomer to the open-source AI world, I began using 
**Ollama** to run tests on my local machines and discovered the original project. Since I spend more time 
working in Linux and macOS development environments, I decided to fork the original project and add 
support for these platforms.

Contributions are welcome and must comply with the [MIT License](LICENSE).

## Getting Started

1. **Fork the repository** on GitHub or your terminal using `gh` cli:

    ```bash
    gh repo fork palmaguer/ollama-monitor
    ```

2. Clone the forked repository
    ```bash
    git clone https://github.com/<your-username>/ollama-monitor.git
    ```

2. Set up your environment:

   - **Windows:** Install Visual Studio 2022 (https://visualstudio.microsoft.com/) with C++ support and CMake (https://cmake.org/).

   - **Linux/WSL2:** Install dependencies:
      
      ```bash
      sudo apt install build-essential cmake libcurl4-openssl-dev
      ```

3. Build the project:

    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```
   
4. Test binary.

---

## Project Structure

```
ollama-monitor/
├── CMakeLists.txt           # Cross-platform build config
├── CHANGELOG.md             # Release history
├── LICENSE                  # MIT License
├── include/
│   ├── ollama_client.h      # Ollama API client
│   ├── gpu_monitor.h        # GPU monitoring abstraction
│   └── console_ui.h         # Console UI rendering
└── src/
    ├── main.cpp             # Entry point & CLI parsing
    ├── ollama_client.cpp    # HTTP client (WinHTTP / libcurl)
    ├── gpu_monitor.cpp      # NVML / DXGI GPU monitoring
    └── console_ui.cpp       # Top-style terminal display
```

## Coding Guidelines

- **Code Style:** Follow the existing code structure and naming conventions. Use consistent indentation (4 spaces) and keep functions concise.

- **Documentation:** Add comments to complex logic, update the README/CONTRIBUTING.md or create new markdown files under `docs/` as needed.

- **Testing:** Write tests for new features or bug fixes if applicable. Ensure existing tests pass before submitting a PR.

- Tools: Use clang-format (https://clang.llvm.org/docs/ClangFormat.html) for code formatting (if configured).

## Commit Message Conventions

Use Conventional Commits (https://www.conventionalcommits.org/) for clarity:

  - **feat:** *New feature*
  - **fix:** *Bug fix*
  - **docs:** *Documentation changes*
  - **style:** *Code style/Formatting*
  - **refactor:** *Code refactoring*
  - **test:** *Add or update tests*
  - **chore:** *Build process or auxiliary tool changes*

Example:
  ```bash
  feat(gpu_monitor): add support for AMD GPU metrics
  ```


## Pull Request Process

1. **Branching**: Create a descriptive branch for your changes (e.g., `feat/add-amd-support` / `feat-add-amd-support`).

2. **PR Submission**: Submit a PR to the `main` branch with a clear title and description.

3. **Requirements**:
   - Include tests for new features.
   - Ensure CI/CD builds pass (Linux/Windows).
   - Address feedback from reviewers.

4. **Approval**: PRs require at least one approve from maintainers before merging.


## Release Notes and CHANGELOG

- Releases are created by CI/CD when a tag like `v*` is pushed.

- The release workflow tries to extract release notes from the matching section in `CHANGELOG.md` using the tag name (for example, `## [v1.1.2]`).

- If no matching section is found, CI/CD falls back to GitHub auto-generated notes.

- Maintainers can enable strict mode with `STRICT_CHANGELOG_RELEASE_NOTES=true` in the release workflow to fail releases when notes are missing.

- Contributors should update `CHANGELOG.md` for user-facing changes so releases include curated notes instead of fallback notes.


## Community & Code of Conduct

- **Communication:** Be respectful and constructive in all interactions. Report issues via GitHub Issues (https://github.com/palmaguer/ollama-monitor/issues).

- **Code of Conduct:** Follow the MIT License (LICENSE) and maintain a welcoming environment. 

See the original project (https://github.com/yonie/ollama-monitor/blob/main/CODE_OF_CONDUCT.md) for guidelines.


## License

This project is licensed under the MIT License (LICENSE). By contributing, you agree that your submissions are licensed under the same terms.


## Acknowledgments

- Original work by yonie (https://github.com/yonie).

- Contributions from the community.

- Dependencies are declared in [README.md](./README.md)

---

Thank you for contributing! Your efforts help make this tool better for all users.
