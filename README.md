A project to test CMake & Ninja failing to rebuild .proto change on the first try.

# Build

## Windows
```
"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32.bat"
mkdir build
cd build
d:\bin\cmake-3.16.1-win64-x64\bin\cmake.exe  -G Ninja -DCMAKE_MAKE_PROGRAM="d:\bin\ninja\ninja.exe" ..
```

## Linux
```
mkdir build
cd build
CXX=/usr/bin/g++-9 CC=/usr/bin/gcc-9 cmake -G Ninja ..
ninja -v -d explain
```
