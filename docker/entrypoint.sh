#!/usr/bin/env bash
set -euo pipefail

Z21SIM_BIN=${Z21SIM_BIN:-/opt/z21sim/bin/Z21Sim}
WEB_PORT=${WEB_PORT:-6080}
VNC_PORT=${VNC_PORT:-5900}
DISPLAY_NUMBER=${DISPLAY_NUMBER:-0}
GEOMETRY=${GEOMETRY:-1920x1080}
DEPTH=${DEPTH:-24}
NOVNC_WEB=${NOVNC_WEB:-/usr/share/novnc}
NOVNC_PROXY=${NOVNC_PROXY:-}

if [ ! -x "$Z21SIM_BIN" ]; then
  echo "Z21Sim binary not found at $Z21SIM_BIN" >&2
  exit 1
fi

# Resolve noVNC proxy helper
if [ -z "$NOVNC_PROXY" ]; then
  if [ -x "${NOVNC_WEB}/utils/novnc_proxy" ]; then
    NOVNC_PROXY="${NOVNC_WEB}/utils/novnc_proxy"
  else
    NOVNC_PROXY="$(command -v novnc_proxy || true)"
  fi
fi
if [ -z "$NOVNC_PROXY" ]; then
  echo "noVNC proxy script not found (looked in ${NOVNC_WEB}/utils/novnc_proxy)" >&2
  exit 1
fi

export DISPLAY=:${DISPLAY_NUMBER}
export QT_QPA_PLATFORM=xcb
export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp/runtime-z21}
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR" || true
mkdir -p /tmp/.X11-unix

cleanup() {
  for pid in ${APP_PID:-} ${NOVNC_PID:-} ${X11VNC_PID:-} ${FLUXBOX_PID:-} ${XVFB_PID:-}; do
    if [ -n "${pid:-}" ] && kill -0 "$pid" >/dev/null 2>&1; then
      kill "$pid" >/dev/null 2>&1 || true
    fi
  done
}
trap cleanup EXIT

# Start virtual framebuffer
Xvfb :${DISPLAY_NUMBER} -screen 0 ${GEOMETRY}x${DEPTH} -ac +extension RANDR >/tmp/xvfb.log 2>&1 &
XVFB_PID=$!

sleep 1

# Lightweight window manager for decent defaults
fluxbox >/tmp/fluxbox.log 2>&1 &
FLUXBOX_PID=$!

# Expose the X display over VNC
x11vnc -display ${DISPLAY} -nopw -forever -shared -rfbport ${VNC_PORT} -quiet >/tmp/x11vnc.log 2>&1 &
X11VNC_PID=$!

# Serve VNC over WebSockets + static assets
"${NOVNC_PROXY}" --vnc localhost:${VNC_PORT} --listen ${WEB_PORT} --web ${NOVNC_WEB} >/tmp/novnc.log 2>&1 &
NOVNC_PID=$!

# Launch simulator
cd "$(dirname "$Z21SIM_BIN")"
"$Z21SIM_BIN" "$@" &
APP_PID=$!

wait $APP_PID
