# Worlds Best Text Editor 2

## VCPKG

This must be done on the host machine. If it already exists, these steps do not need to be repeated.

```shell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
```

Add the following to `.bashrc`, or just export in the current terminal session.

```shell
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```


x11
sudo apt install libx11-dev libxft-dev libxext-dev

wayland
sudo apt install libwayland-dev libxkbcommon-dev libegl1-mesa-dev

ibus
sudo apt install libibus-1.0-dev

all of these were eventually installed
sudo apt install libwayland-dev wayland-protocols libxkbcommon-dev libegl1-mesa-dev libgl1-mesa-dev libdrm-dev libgbm-dev libx11-dev libxrandr-dev libxcursor-dev libxinerama-dev libxi-dev libxext-dev

// might not be used?
sudo apt install libdecor-0-dev

sudo apt install libtool