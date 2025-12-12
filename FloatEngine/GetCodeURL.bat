@echo off
setlocal enabledelayedexpansion

REM 设置输出文件名
set OUTPUT_FILE=codes.txt

REM 删除已存在的输出文件（如果存在）
if exist %OUTPUT_FILE% del %OUTPUT_FILE%

REM 设置GitHub仓库基础URL
set BASE_URL=https://github.com/Fls-Float/FloatEngine/blob/master/FloatEngine/

REM 遍历当前目录及子目录中的所有C++文件
for /r %%i in (*.cpp *.hpp *.h *.cc *.cxx *.hxx) do (
    REM 获取相对路径
    set "filepath=%%i"
    set "filepath=!filepath:%CD%\=!"
    
    REM 将反斜杠替换为正斜杠（GitHub URL格式）
    set "filepath=!filepath:\=/!"
    
    REM 将文件路径写入输出文件
    echo !BASE_URL!!filepath! >> %OUTPUT_FILE%
)

echo 已完成！所有C++文件已导出到 %OUTPUT_FILE%
pause