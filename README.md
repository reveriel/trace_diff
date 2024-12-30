# Trace Diff

[![CI](https://github.com/reveriel/trace_diff/actions/workflows/ci.yml/badge.svg)](https://github.com/reveriel/trace_diff/actions)
[![codecov](https://codecov.io/gh/reveriel/trace_diff/branch/main/graph/badge.svg)](https://codecov.io/gh/reveriel/trace_diff)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Bazel](https://img.shields.io/badge/Build%20with-Bazel-43A047.svg)](https://bazel.build/)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/reveriel/trace_diff/pulls)
[![Code Style](https://img.shields.io/badge/code%20style-clang--format-blue.svg)](https://clang.llvm.org/docs/ClangFormat.html)


TraceDiff 考虑**库重写的 diff 问题， 在不需要 diff 时，也可以作为 普通的 log 使用

diff 方式是用同样的方式调用 新库 和 旧库， 并比较结果。

在 最终结果不一致时，需要打印 库的内部 数据， 方便排查问题。

本 library 提供打印 log 的接口， 方便排查问题。

使用本库时需要确保请求并发数为 1. 避免 不同请求的 log 交叉打印。


## Usage

1. lib 入口

``` cpp
TraceDiff::set_log_prefix("p1");
call_old_lib();
TraceDiff::set_log_prefix("p2");
call_new_lib();
```

The counter will automatically increment when we start a new cycle of prefixes. For example:

``` cpp
TraceDiff::set_log_prefix("a");  // counter = 1
TraceDiff::set_log_prefix("b");  // counter = 1
TraceDiff::set_log_prefix("c");  // counter = 1
TraceDiff::set_log_prefix("a");  // counter = 2, starts new cycle
TraceDiff::set_log_prefix("b");  // still counter = 2
TraceDiff::set_log_prefix("c");  // still counter = 2
TraceDiff::set_log_prefix("a");  // counter = 3, starts new cycle
```

2. new_lib and old_lib 中 使用

``` cpp
TRACE_DIFF_LOG("field_name", field_value);
```

会在 `tracediff_p1.log`, `tracediff_p2.log` 中打印 如下内容

cnt:prefix:field_name: field_value

例如：

```
1:p1:gids_before  1,3,4,5
1:p1:gids_after: 1,3
1:p2:gids_before: 1,2,4,5
1:p2:gids_after: 1,3
2:p1:gids_before: 1,3,4,5
2:p1:gids_after: 1,3
2:p2:gids_before: 1,2,4,5
2:p2:gids_after: 1,3
```


用户负责确保两个 lib 使用同样的 TRACE_DIFF_LOG 宏。
使得 cnt 值相同时， 不同 prefix 的 log 条数是相同的， 顺序可能有变化。

然后使用本目录下的脚本对 log 文件进行 diff 的分析, 我们会将 后者的 log 顺序调整成 与 前者一致， 便于分析。


## Enabling Logging

To enable logging, define the `ENABLE_TRACE_DIFF_LOG` macro either in your code or as a compilation flag:

```cpp
#define ENABLE_TRACE_DIFF_LOG
```


> **Note**: To make the badges work in your fork:
> 1. Replace "reveriel" in badge URLs with your GitHub username
> 2. Enable GitHub Actions in your repository
> 3. Set up [Codecov](https://codecov.io) for your repository:
>    - Sign up on Codecov using your GitHub account
>    - Add your repository to Codecov
>    - Add `CODECOV_TOKEN` to your repository secrets in GitHub
>    - The token is required for the Codecov GitHub Action v5
> 4. The CI workflow will automatically upload coverage reports to Codecov


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
