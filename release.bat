@echo off
:: 定义源目录和目标目录
set SOURCE_HEADERS=.\FloatEngine
set SOURCE_LIBS=.\x64\Release
set TARGET_INCLUDE=C:\floatengine\include
set TARGET_LIB=C:\floatengine\lib

:: 创建目标目录
if not exist "%TARGET_INCLUDE%" mkdir "%TARGET_INCLUDE%"
if not exist "%TARGET_INCLUDE%\floatengine" mkdir "%TARGET_INCLUDE%\floatengine"
if not exist "%TARGET_LIB%" mkdir "%TARGET_LIB%"

:: 拷贝 floatengine.h 到目标 include 目录
echo Copying floatengine.h to %TARGET_INCLUDE%
copy /Y "%SOURCE_HEADERS%\floatengine.h" "%TARGET_INCLUDE%"

:: 拷贝其他 .h 文件到 floatengine 目录（排除 floatengine.h）
echo Copying other .h files to %TARGET_INCLUDE%\floatengine
for %%f in ("%SOURCE_HEADERS%\*.h") do (
    if /I not "%%~nxf"=="floatengine.h" (
        copy /Y "%%f" "%TARGET_INCLUDE%\floatengine"
    )
)

:: 拷贝库文件到目标 lib 目录
echo Copying libraries to %TARGET_LIB%
copy /Y "%SOURCE_LIBS%\floatengine.lib" "%TARGET_LIB%"
copy /Y "physfs.lib" "%TARGET_LIB%"

echo All files copied successfully!
