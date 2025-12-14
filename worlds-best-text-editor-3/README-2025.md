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

