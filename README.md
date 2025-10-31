# {{PROJECT_NAME}}

A c++ project with imgui and cmake.

## Setup

run

```bash
./init_name.py
```

to set the project name, after this running it again will not do anything. You can roll back with git:

```bash
git checkout *
```

for debugging, run the `setup.sh` to link the required files into the build directory (for creating bundles this isn't needed).

## Building

### Linux

building just the executable (for debugging):

```bash
./setup.sh # only needed once
cmake -B ./build .
cmake build ./build
```

building the appimage:

```bash
./build-appimage.sh
```

### Windows

The windows builds are cross compiled on linux with `mingw-w64-gcc`.

```bash
./build-win-installer.sh
```
