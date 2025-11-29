Package: bzip2[core,tool]:x64-windows@1.0.8#6

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
-- Using cached bzip2-1.0.8.tar.gz
-- Cleaning sources at E:/Github Projects/vcpkg-master/buildtrees/bzip2/src/bzip2-1-336d4794a3.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source E:/Github Projects/vcpkg-master/downloads/bzip2-1.0.8.tar.gz
-- Applying patch fix-import-export-macros.patch
-- Using source at E:/Github Projects/vcpkg-master/buildtrees/bzip2/src/bzip2-1-336d4794a3.clean
-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
CMake Error at scripts/cmake/vcpkg_execute_build_process.cmake:134 (message):
    Command failed: "D:/Program Files/CMake/bin/cmake.exe" --build . --config Release --target install -- -v -j9
    Working Directory: E:/Github Projects/vcpkg-master/buildtrees/bzip2/x64-windows-rel
    See logs for more information:
      E:\Github Projects\vcpkg-master\buildtrees\bzip2\install-x64-windows-rel-out.log

Call Stack (most recent call first):
  E:/FloatStudio/Engine Related/FloatEngine/vcpkg_installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_build.cmake:74 (vcpkg_execute_build_process)
  E:/FloatStudio/Engine Related/FloatEngine/vcpkg_installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_install.cmake:16 (vcpkg_cmake_build)
  ports/bzip2/portfile.cmake:31 (vcpkg_cmake_install)
  scripts/ports.cmake:206 (include)



```

<details><summary>E:\Github Projects\vcpkg-master\buildtrees\bzip2\install-x64-windows-rel-out.log</summary>

```
Change Dir: 'E:/Github Projects/vcpkg-master/buildtrees/bzip2/x64-windows-rel'

Run Build Command(s): "E:/Github Projects/vcpkg-master/downloads/tools/ninja/1.13.1-windows/ninja.exe" -v -v -j9 install
[1/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\crctable.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\crctable.c"
[2/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\randtable.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\randtable.c"
[3/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\huffman.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\huffman.c"
[4/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\crctable.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\crctable.c"
[5/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\huffman.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\huffman.c"
[6/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\randtable.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\randtable.c"
[7/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\bzlib.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\bzlib.c"
[8/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\blocksort.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\blocksort.c"
[9/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\bzip2.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\bzip2.c"
[10/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\blocksort.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\blocksort.c"
[11/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\crctable.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\crctable.c"
[12/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\huffman.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\huffman.c"
[13/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\bzip2recover.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\bzip2recover.c"
[14/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\randtable.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\randtable.c"
[15/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\bzlib.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\bzlib.c"
[16/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\blocksort.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\blocksort.c"
[17/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2recover.dir\bzlib.c.obj /FdCMakeFiles\bzip2recover.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\bzlib.c"
[18/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -DBZ_BUILD_DLL -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -Dbz2_EXPORTS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bz2.dir\decompress.c.obj /FdCMakeFiles\bz2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\decompress.c"
[19/27] "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\cl.exe"   -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS  /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7  -MD /showIncludes /FoCMakeFiles\bzip2.dir\decompress.c.obj /FdCMakeFiles\bzip2.dir\ /FS -c "E:\Github Projects\vcpkg-master\buildtrees\bzip2\src\bzip2-1-336d4794a3.clean\decompress.c"
ninja: build stopped: interrupted by user.
```
</details>

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
