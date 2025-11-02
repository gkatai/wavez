# wavez
Simple tower defense written in C and raylib

## Clone

Clone the repository with submodules:
```bash
git clone --recurse-submodules https://github.com/gkatai/wavez.git
```

If you already cloned without submodules:
```bash
git submodule update --init --recursive
```

## Building (Native)

### Debug Build
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

### Release Build
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Building (WebAssembly)

### Debug Build
```bash
mkdir -p build && cd build
emcmake cmake ..
cmake --build .
```

### Release Build
```bash
mkdir -p build && cd build
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

Or use the custom target:
```bash
cmake --build . --target release
```

### Running WebAssembly Build
```bash
cd build
cmake --build . --target run
```

This will start a local web server using `emrun`.
