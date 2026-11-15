# Arfetch

A lightweight and fast system fetch written in modern C++ exclusively for **Linux**.

[![Static Release](https://github.com/Ars-byte/Arfetch/actions/workflows/release.yml/badge.svg)](https://github.com/Ars-byte/Arfetch/actions/workflows/release.yml)
[![Linux Only](https://img.shields.io/badge/OS-Linux%20Only-orange.svg)](#)

> **Note:** Arfetch is designed specifically for Linux environments (X11 & Wayland). Windows and macOS are not supported.

## Download Prebuilt Static Binary

Every time the code is updated, a standalone static Linux binary is automatically built and published:

```bash
# Download latest static binary
curl -LO https://github.com/Ars-byte/Arfetch/releases/download/continuous/Arfetch-linux-x86_64
chmod +x Arfetch-linux-x86_64
./Arfetch-linux-x86_64
```

## Features

- **OS & Kernel:** Distro name and kernel release
- **Uptime & Date:** System running time and local timestamp
- **Packages:** Automatic detection for xbps, dpkg, pacman, rpm, and qlist
- **Hardware:** CPU model, GPU vendor identification, memory usage
- **Desktop Environment & WM:** Automatic detection of SwayFX, Sway, Hyprland, GNOME, KDE Plasma, XFCE, etc.
- **Battery & Shell:** Power status and current user shell

## Build

### Quick compilation
```bash
g++ arfetch.cpp -o Arfetch
./Arfetch
```

### Using Makefile
```bash
# Standard dynamic build
make

# Build standalone static binary (no shared library dependencies)
make static

# Optional: Install to /usr/local/bin
sudo make install
```

## Preview

<img width="451" height="321" alt="image" src="https://github.com/user-attachments/assets/1bb196dc-66d1-474c-a5b4-07adfc6555b9" />

