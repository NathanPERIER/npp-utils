
# CPP utils

These are some utilities I have used in personal projects and find quite useful. It is not necessarily intended to be included as a library, as some better alternatives may exist in libraries like boost or folly. There are mainly two goals with this repository :
- To provide small functionalities that can be integrated in any project while relying only on the stdlib (maybe also `fmt` for conveniency, but this can usually be replaced by `std::ostream` quite easily).
- To showcase ideas that can be copied and modified to make a specialised component adapted to the needs of a projects.

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

