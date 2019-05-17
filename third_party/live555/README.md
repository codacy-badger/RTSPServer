# live555
A mirror of the live555 source code.
For documentation and instructions for building this software,
see <http://www.live555.com/liveMedia/>

# Build
Build project using cmake.

If you do not want shared library, you should modify value of `shared` from `OFF` to `ON`.

```cmake
mkdir build && cmake .. && make -j2 # static library by default
```
