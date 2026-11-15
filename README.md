# Arfetch

A lightweight and fast system fetch written in modern C++ exclusively for **Linux**.

> **Note:** Arfetch is designed specifically for Linux environments (X11 & Wayland). Windows and macOS are not supported.

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

<img width="485" height="269" alt="image" src="https://github.com/user-attachments/assets/537d6b44-9dca-453d-a926-4c5e9d20a6a6" />
