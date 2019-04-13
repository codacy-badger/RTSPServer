[![Commitizen friendly](https://img.shields.io/badge/commitizen-friendly-brightgreen.svg)](https://github.com/kgbook/RTSPServer)

# RTSPServer

# RTSPClient
Pull stream from RTSPServer using FFmpeg, support H.264/H.265 wrapper into MP4 container.

# Dependencies
- [FFmpeg](https://ffmpeg.org/)
- [live555](http://www.live555.com/)
- [spdlog](https://github.com/gabime/spdlog)
- [Boost](https://www.boost.org/) at least 1.66.0
- [fmt](https://github.com/fmtlib/fmt)

# Build
```
mkdir build && cd build && cmake .. && make -j2
```

# TODO
- RTSPServer