Package: miniaudio:x64-windows-static@0.11.23

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.50.35718.0
- CMake Version: 4.2.0
-    vcpkg-tool version: 2025-11-19-da1f056dc0775ac651bea7e3fbbf4066146a55f3
    vcpkg-scripts version: unknown

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading https://github.com/mackron/miniaudio/archive/0.11.23.tar.gz -> mackron-miniaudio-0.11.23.tar.gz
warning: Download https://github.com/mackron/miniaudio/archive/0.11.23.tar.gz failed -- retrying after 1000ms
mackron-miniaudio-0.11.23.tar.gz.28268.part: error: download from https://github.com/mackron/miniaudio/archive/0.11.23.tar.gz had an unexpected hash
note: Expected: b12566432e0167082dd9ad5b5c5fc3d80a80c7803016a59c670f5fb3436c2db8b16411e3f10571eafbf6791c53b761c3deeabb22b6329f80bbe891c760365c3c
note: Actual  : de956cf7f4e80495d51631837e9eabf0168986041fa9e6ef1544051ebf1c2019b5f88c931bdd6ce8413dde44661ca6e3e1ff7e29772d12789cc63303835d0078
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:136 (message):
  Download failed, halting portfile.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_from_github.cmake:120 (vcpkg_download_distfile)
  ports/miniaudio/portfile.cmake:2 (vcpkg_from_github)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "minizip-ng",
    "raylib"
  ]
}

```
</details>
