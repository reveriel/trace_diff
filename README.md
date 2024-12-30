# Bazel C++ Project Template

[![CI](https://github.com/reveriel/bazel_template/actions/workflows/ci.yml/badge.svg)](https://github.com/reveriel/bazel_template/actions)
[![codecov](https://codecov.io/gh/reveriel/bazel_template/branch/main/graph/badge.svg)](https://codecov.io/gh/reveriel/bazel_template)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Bazel](https://img.shields.io/badge/Build%20with-Bazel-43A047.svg)](https://bazel.build/)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/reveriel/bazel_template/pulls)
[![Code Style](https://img.shields.io/badge/code%20style-clang--format-blue.svg)](https://clang.llvm.org/docs/ClangFormat.html)

A modern C++ project template using Bazel build system, featuring comprehensive testing, code coverage, and development tools integration.

> **Note**: To make the badges work in your fork:
> 1. Replace "reveriel" in badge URLs with your GitHub username
> 2. Enable GitHub Actions in your repository
> 3. Set up [Codecov](https://codecov.io) for your repository:
>    - Sign up on Codecov using your GitHub account
>    - Add your repository to Codecov
>    - Add `CODECOV_TOKEN` to your repository secrets in GitHub
>    - The token is required for the Codecov GitHub Action v5
> 4. The CI workflow will automatically upload coverage reports to Codecov

## Features

- Modern C++17 support
- Bazel build system with Bzlmod dependency management
- GoogleTest integration for unit testing
- Code coverage reporting with LCOV
- Multiple build configurations (Debug/Release)
- Sanitizer support (ASan, TSan, UBSan)
- VSCode DevContainer configuration
- GitHub Actions CI integration
- Clang tooling support

## Prerequisites

- Docker and VSCode with Remote-Containers extension (for DevContainer)
- Or locally:
  - Bazel 6.0+ (or Bazelisk)
  - C++17 compatible compiler
  - Python 3.x (for coverage report viewing)
  - LCOV (for coverage report generation)

## Quick Start with DevContainer

1. Install Docker and VSCode with Remote-Containers extension
2. Open this project in VSCode
3. Click "Reopen in Container" when prompted
4. Wait for the container to build and initialize

## Build Instructions

### Basic Build Commands

```bash
# Build everything
bazel build //...

# Build and run main binary
bazel run //:main

# Generate compile_commands.json for IDE support
bazel run :refresh_compile_commands
```

### Build Configurations

```bash
# Debug build
bazel build --config=debug //...

# Release build
bazel build --config=release //...

# Build with sanitizers
bazel build --config=asan //...  # Address Sanitizer
bazel build --config=tsan //...  # Thread Sanitizer
bazel build --config=ubsan //... # Undefined Behavior Sanitizer
```

## Testing

The project uses GoogleTest for unit testing. Tests are located in the `test/` directory.

```bash
# Run all tests
bazel test //...

# Run specific test
bazel test //:calculator_test

# Run tests with sanitizers
bazel test --config=asan //...
bazel test --config=tsan //...
bazel test --config=ubsan //...
```

## Code Coverage

Code coverage is automatically generated and uploaded to Codecov during CI runs. To generate coverage reports locally:

```bash
# Generate coverage report for all tests
bazel coverage //...

# Generate HTML report
genhtml "$(bazel info output_path)/_coverage/_coverage_report.dat" -o coverage_report

# View the report (starts a local server)
cd coverage_report && python3 -m http.server 8000
```

Access the coverage report at `http://localhost:8000` in your web browser.

The coverage configuration:
- Automatically runs during CI with GitHub Actions
- Uses Codecov Action v5 for report uploads
- Focuses on project code using `--instrumentation_filter`
- Excludes external dependencies
- Generates reports in LCOV format
- Shows line, branch, and function coverage

## Project Structure

```
.
├── .devcontainer/        # Development container configuration
├── .github/              # GitHub Actions workflows
├── include/              # Public headers
│   └── calculator/       # Namespace-based organization
├── src/                  # Source files
│   └── calculator/       # Implementation files
├── test/                 # Test files
│   └── calculator/       # Test implementations
├── BUILD.bazel          # Main build rules
├── MODULE.bazel         # Bazel module definition
└── .bazelrc            # Bazel configuration
```

## Development Tools

### VSCode Integration

The DevContainer comes pre-configured with:
- C++ extension
- Bazel extension
- Clang-format
- Clang-tidy
- LLDB debugger

### Available Commands

- Build: `Ctrl+Shift+B` or `CMD+Shift+B`
- Run Tests: Via Testing sidebar
- Debug: F5 (after selecting a target)

### Code Formatting

The project uses clang-format for code formatting. Format your code with:
```bash
find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i
```

## Debugging

The project is configured for debugging with VSCode. Two debug configurations are provided:
- "Debug Main" for debugging the main program
- "Debug Tests" for debugging the test suite

### Prerequisites
- GDB (installed in DevContainer)
- VSCode C/C++ extension
- VSCode Bazel extension

### Starting a Debug Session

1. Set breakpoints by clicking on the line numbers in your source files
2. Press F5 or select "Run and Debug" from the sidebar
3. Choose either "Debug Main" or "Debug Tests" from the dropdown
4. The debugger will stop at your breakpoints

### Available Debug Commands
- F5: Continue
- F10: Step Over
- F11: Step Into
- Shift+F11: Step Out
- Ctrl+Shift+F5: Restart
- Shift+F5: Stop

### Debug Features
- Variable inspection in the Debug sidebar
- Watch expressions
- Call stack viewing
- Memory inspection
- Breakpoint conditions
- Debug console for GDB commands

### Debug Configurations
The `.vscode/launch.json` includes:
```json
{
    "configurations": [
        {
            "name": "Debug Main",
            "program": "${workspaceFolder}/bazel-bin/main",
            // Debug configuration for main program
        },
        {
            "name": "Debug Tests",
            "program": "${workspaceFolder}/bazel-bin/calculator_test",
            // Debug configuration for tests
        }
    ]
}
```

Both configurations automatically build with debug symbols before starting the debug session.

## Continuous Integration

GitHub Actions automatically:
- Builds the project in Debug and Release modes
- Runs all tests
- Performs sanitizer checks
- Generates coverage reports
- Uploads coverage to Codecov using Codecov Action v5
- Uses latest GitHub Actions including:
  - actions/checkout@v3
  - actions/upload-artifact@v4
  - codecov/codecov-action@v5

The CI pipeline is configured to fail if:
- Any tests fail
- Coverage upload fails
- Build errors occur

## License

[Add your license here]
