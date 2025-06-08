# raymino

configurable tetris build with raylib in C++ for desktop & web

[play web version here](https://sgs.moe/raymino)

![preview pictures](raymino.png)

## build

make sure you have git, cmake, a c++ compiler
and [raylib#build-and-installation](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation)

```bash
git clone https://github.com/StillGreen-san/raymino
cd raymino
```

### destop

```
mkdir build-exe
cmake -S . -B build-exe
cmake --build build-exe --config release --target raymino
```

### web

for emscripten [see here](https://emscripten.org/docs/getting_started/downloads.html),
on windows you also need [ninja](https://ninja-build.org/)

```
mkdir build-web
emcmake cmake -S . -B build-web -DPLATFORM=Web
cmake --build build-web --config release --target raymino
```

to properly load wasm the site needs to be hosted by a webserver

eg `python -m http.server`

## dependencies

_(pulled in via [CPM](https://github.com/cpm-cmake) [MIT])_

- [raylib](https://github.com/raysan5/raylib) [Zlib]
- [raylib-cpp](https://github.com/RobLoach/raylib-cpp) [Zlib]
- [raygui](https://github.com/raysan5/raygui) [Zlib]
- [rres](https://github.com/raysan5/rres) [MIT] (currently unused)
- [magic_enum](https://github.com/Neargye/magic_enum) [MIT]

### dev only

- [Catch2](https://github.com/catchorg/Catch2) [BSL]
