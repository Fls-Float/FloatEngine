Package: liblzma:x64-windows@5.8.1

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
Downloading https://github.com/tukaani-project/xz/archive/v5.8.1.tar.gz -> tukaani-project-xz-v5.8.1.tar.gz
tukaani-project-xz-v5.8.1.tar.gz.23076.part: error: download from https://github.com/tukaani-project/xz/archive/v5.8.1.tar.gz had an unexpected hash
note: Expected: 2f51fb316adb2962e0f2ef6ccc8b544cdc45087b9ad26dcd33f2025784be56578ab937c618e5826b2220b49b79b8581dcb8c6d43cd50ded7ad9de9fe61610f46
note: Actual  : e931b6953fa2879e70215f0e5dfa4abc39eff101dfd07ce15f7dfda8cdd0286a1d764b75b112373c28a04a91eeadbcd7d10c5d3ef09ce832d98a1e3f4b971e63
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:136 (message):
  Download failed, halting portfile.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_from_github.cmake:120 (vcpkg_download_distfile)
  ports/liblzma/portfile.cmake:1 (vcpkg_from_github)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "minizip-ng"
  ]
}

```
</details>
