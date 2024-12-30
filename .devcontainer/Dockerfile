FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

# Install additional packages
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        lcov \
        gcovr \
        clang \
        llvm \
        python3-pip \
        curl \
        gnupg \
        gdb \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Bazelisk (architecture-independent Bazel wrapper)
RUN curl -Lo /usr/local/bin/bazel https://github.com/bazelbuild/bazelisk/releases/download/v1.19.0/bazelisk-linux-$(dpkg --print-architecture) \
    && chmod +x /usr/local/bin/bazel

# Create a non-root user if it doesn't exist
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN if ! getent group $USERNAME > /dev/null 2>&1; then \
        groupadd --gid $USER_GID $USERNAME; \
    fi && \
    if ! getent passwd $USERNAME > /dev/null 2>&1; then \
        useradd --uid $USER_UID --gid $USER_GID -m $USERNAME; \
    fi && \
    apt-get update && \
    apt-get install -y sudo && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
