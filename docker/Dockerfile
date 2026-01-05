## Multi-stage build that compiles the Qt-based simulator and serves its UI over noVNC

# ---------- Builder ----------
FROM ubuntu:24.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive

# Build dependencies for the simulator
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      gcc-14 \
      g++-14 \
      build-essential \
      ca-certificates \
      cmake \
      curl \
      git \
      libgl1-mesa-dev \
      libglu1-mesa-dev \
      libxkbcommon-dev \
      libxkbcommon-x11-dev \
      libxcb-cursor-dev \
      ninja-build \
      pkg-config \
      python3 \
      qt6-base-dev \
      qt6-base-dev-tools \
      qt6-tools-dev \
      qt6-tools-dev-tools \
      qt6-svg-dev \
      && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src/Z21
COPY . .

# Configure and build the simulator (Debug avoids the Release-only Qt-from-source path)
RUN rm -rf build && \
    QT_VERSION=$(pkg-config --modversion Qt6Core | tr -d '\n') && \
    CC=gcc-14 CXX=g++-14 \
    cmake -G Ninja -B build -S . \
      -DCMAKE_BUILD_TYPE=Debug \
      -DZ21_SIM_HIDE_CONFIDENTIAL_SETTINGS=ON \
      -DQT_VERSION=${QT_VERSION} && \
    cmake --build build --target Z21Sim

# Collect the runtime payload
RUN install -d /opt/z21sim/bin && \
    install -m 0755 build/examples/sim/Z21Sim /opt/z21sim/bin/Z21Sim && \
    cp -r data /opt/z21sim/data


# ---------- Runtime ----------
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

# Runtime dependencies: Qt libs, headless X server, VNC + noVNC
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      ca-certificates \
      libasan8 \
      libubsan1 \
      libstdc++6 \
      fluxbox \
      fonts-dejavu-core \
      libgl1 \
      libqt6core6 \
      libqt6gui6 \
      libqt6network6 \
      libqt6widgets6 \
      libqt6svg6 \
      libxcb-cursor0 \
      novnc \
      websockify \
      x11vnc \
      xvfb \
      && \
    rm -rf /var/lib/apt/lists/*

ENV Z21SIM_HOME=/opt/z21sim \
    DISPLAY=:0 \
    WEB_PORT=6080 \
    VNC_PORT=5900 \
    GEOMETRY=1920x1080 \
    DEPTH=24

COPY --from=builder /opt/z21sim /opt/z21sim
COPY docker/entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

# Use a non-root user for the app runtime
RUN useradd -ms /bin/bash z21 && chown -R z21:z21 /opt/z21sim
USER z21
WORKDIR /opt/z21sim

EXPOSE 6080 5900
EXPOSE 21105/udp 21106/udp

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
