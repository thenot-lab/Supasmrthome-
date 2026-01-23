FROM ubuntu:20.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    mingw-w64 \
    gcc-multilib \
    git \
    python3
WORKDIR /opt/copilot-365-agent
COPY . /opt/copilot-365-agent
