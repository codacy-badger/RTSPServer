[![Commitizen friendly](https://img.shields.io/badge/commitizen-friendly-brightgreen.svg)](https://github.com/kgbook/RTSPServer)

# RTSPServer
Push H.264/H.265 bitstream using live555.

# RTSPClient
Pull stream from RTSPServer using FFmpeg, support H.264/H.265 wrapper into MP4 container.

# Dependencies
- [FFmpeg](https://ffmpeg.org/)
- [live555](http://www.live555.com/)
- [spdlog](https://github.com/gabime/spdlog)
- [Boost](https://www.boost.org/) at least 1.66.0
- [fmt](https://github.com/fmtlib/fmt)
- [python](https://www.python.org/) 2.7 required
- [cmake](https://cmake.org/) 3.7 required

# Build
```
mkdir build && cd build && cmake .. && make -j2
```

# Code Style
The project use automated checker named **cpplint** to make sure a C++ file follows [Google's C++ style
guide](https://google.github.io/styleguide/cppguide.html).

False positives can be ignored by putting `// NOLINT` at the end of the line or `// NOLINTNEXTLINE` in the previous line.

# TODO
- [FIXME: make_value is ambiguous on macOS Mojave 10.14.4, fmt 5.3.0](https://github.com/fmtlib/fmt/issues/1115)