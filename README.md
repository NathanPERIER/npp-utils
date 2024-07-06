
# CPP utils

## Build

### Setup

#### 1. Clone the repository

```bash
git clone https://github.com/NathanPERIER/cpp-utils.git
cd cpp-utils
```

#### 2. Download subprojects

```bash
meson subprojects download
```

#### 3. Create the build directories

```bash
meson setup build_debug --buildtype=debug
meson setup build_release --buildtype=release
```

### Compilation

```bash
cd build_debug
ninja
```

