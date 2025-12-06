@echo off
setlocal enabledelayedexpansion

:: ========================================================
:: FloatEngine Core Deployer (SAFE MODE - NO SOURCE TOUCH)
:: --------------------------------------------------------
:: 目标: 仅部署编译产物到目标路径。不对项目源码目录做任何修改。
:: --------------------------------------------------------
:: ========================================================

:: 检查是否以管理员身份运行 (UAC 提权)
NET SESSION >nul 2>&1
if %errorlevel% neq 0 goto :runasadmin

:: ================= 配置区域 (只包含路径) =================
set TARGET_ROOT=C:\floatengine
set TARGET_LIB=%TARGET_ROOT%\lib
set TARGET_INCLUDE=%TARGET_ROOT%\include
set TARGET_INCLUDE_MODULE=%TARGET_INCLUDE%\FloatEngine
:: ------------------------------------------
set BUILD_ROOT=.\build
set ENGINE_CORE_LIB_SRC=%BUILD_ROOT%\Release\FloatEngine.lib
set ENGINE_SRC_DIR=.\FloatEngine
:: ===========================================

echo.
echo ========================================================
echo  [1/4] Deploying FloatEngine Core (SAFE MODE)...
echo ========================================================

:: 0. 检查环境和文件 (确保源码和编译产物存在)
where robocopy.exe >nul 2>nul
if %errorlevel% neq 0 goto :error_env_robocopy
if not exist "%ENGINE_CORE_LIB_SRC%" goto :error_no_lib
if not exist "%ENGINE_SRC_DIR%" goto :error_no_src

:: 1. 清理并创建目标目录 (只清理目标目录，不清理源码或 build)
echo [1/4] Cleaning and creating destination directories...
if exist "%TARGET_ROOT%" rmdir /s /q "%TARGET_ROOT%"
mkdir "%TARGET_LIB%"
mkdir "%TARGET_INCLUDE%"
mkdir "%TARGET_INCLUDE_MODULE%"

:: 2. 部署核心 Lib
echo [2/4] Copying Core Library: FloatEngine.lib
copy /Y "%ENGINE_CORE_LIB_SRC%" "%TARGET_LIB%\"
if %errorlevel% neq 0 goto :error_copy

:: 3. 部署核心 Includes (精确分离)
echo [3/4] Copying Core Headers (FloatEngine.h to root, others to /FloatEngine)...

:: 3.1 复制 FloatEngine.h 到 include/ 根目录
copy /Y "%ENGINE_SRC_DIR%\FloatEngine.h" "%TARGET_INCLUDE%\"
if %errorlevel% neq 0 goto :error_copy

:: 3.2 复制其他 *.h/.*hpp 文件到 include/FloatEngine/ 模块目录
robocopy "%ENGINE_SRC_DIR%" "%TARGET_INCLUDE_MODULE%" *.h *.hpp /XF FloatEngine.h /NFL /NDL /NJH /NJS /R:1 /W:1
if %errorlevel% geq 8 goto :error_deploy_copy


echo.
echo ========================================================
echo  [4/4] SUCCESS! FloatEngine Core deployed.
echo  !!! NO SOURCE FILES WERE MODIFIED !!!
echo  Primary Header: %TARGET_INCLUDE%\FloatEngine.h
echo  Module Headers: %TARGET_INCLUDE_MODULE%
echo ========================================================
pause
exit /b 0

:: ... (UAC 提权代码和错误处理代码保持不变) ...

:runasadmin
echo Set UAC = CreateObject^("Shell.Application"^) > "%TEMP%\getadmin.vbs"
echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%TEMP%\getadmin.vbs"
"%TEMP%\getadmin.vbs"
del "%TEMP%\getadmin.vbs"
exit /b 0

:error_env_robocopy
echo [FATAL ERROR] 'robocopy.exe' not found.
pause
exit /b 1

:error_no_lib
echo [FATAL ERROR] Cannot find compiled core lib at: %ENGINE_CORE_LIB_SRC%. Please run "cmake --build build --config Release" first.
pause
exit /b 1

:error_no_src
echo [FATAL ERROR] Cannot find source directory ".\FloatEngine". Check project structure.
pause
exit /b 1

:error_copy
echo [FATAL ERROR] File copy failed (COPY). Check paths and permissions.
pause
exit /b 1

:error_deploy_copy
echo [FATAL ERROR] Deployment (Robocopy) Failed. Check Robocopy error code and permissions.
pause
exit /b 1