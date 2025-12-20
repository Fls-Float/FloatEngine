import os
import codecs
import argparse
from typing import List

# ====================================================================
# 配置区域
# ====================================================================

FILE_EXTENSIONS = ['.cpp', '.h', '.hpp', '.c']

# 源编码：我们只转换这种编码的文件
SOURCE_ENCODING = 'gbk' 

# 目标编码：统一标准
TARGET_ENCODING = 'utf-8' 

# --------------------------------------------------------------------

def convert_file_encoding(file_path: str, source_enc: str, target_enc: str) -> bool:
    """
    检查文件是否为源编码，如果是则转换并写入目标编码
    """
    try:
        # 1. 【核心检查】尝试以源编码 GBK 读取内容
        # 如果不是 GBK (例如已经是 UTF-8)，这里会抛出 UnicodeDecodeError
        with codecs.open(file_path, 'r', encoding=source_enc) as f:
            content = f.read()
            
        # 2. 如果读取成功，说明文件是 GBK 编码，执行转换
        with codecs.open(file_path, 'w', encoding=target_enc) as f:
            f.write(content)
            
        print(f"[SUCCESS] Converted from {source_enc} to {target_enc}: {file_path}")
        return True
        
    except UnicodeDecodeError:
        # 发生错误，说明文件内容不是 GBK 格式
        # 例如，如果是纯英文或已经是 UTF-8，可能会触发这个错误
        print(f"[INFO] Skipping: {file_path} (File is NOT {source_enc} or already in target encoding)")
    except Exception as e:
        print(f"[FAIL] Unknown Error on {file_path} -> {e}")
    
    return False

def batch_convert_project(root_dir: str, recursive: bool):
    """
    批量转换项目文件的编码
    """
    print(f"--- Starting Batch Encoding Conversion ---")
    print(f"Root Dir: {os.path.abspath(root_dir)}")
    print(f"Source/Target: {SOURCE_ENCODING} -> {TARGET_ENCODING}")
    print("-" * 50)
    
    converted_count = 0
    
    # ... (文件遍历逻辑保持不变) ...

    # -----------------------------------------------------------
    # 遍历的起始目录列表 (文件路径列表)
    # -----------------------------------------------------------
    paths_to_process: List[str] = []

    if recursive:
        for root, _, files in os.walk(root_dir):
            if any(exclude in root for exclude in ['\\.git', 'build', 'vcpkg_installed', 'out']):
                continue
            paths_to_process.extend([os.path.join(root, f) for f in files])
    else:
        for item in os.listdir(root_dir):
            item_path = os.path.join(root_dir, item)
            if os.path.isfile(item_path):
                paths_to_process.append(item_path)
    
    # -----------------------------------------------------------
    # 执行转换
    # -----------------------------------------------------------

    for file_path in paths_to_process:
        ext = os.path.splitext(file_path)[1].lower()
        
        if ext in FILE_EXTENSIONS:
            if convert_file_encoding(file_path, SOURCE_ENCODING, TARGET_ENCODING):
                converted_count += 1
            # 无论成功或失败，继续下一个文件

    print("-" * 50)
    print(f"Total Converted (from {SOURCE_ENCODING}): {converted_count}")
    print("--- Conversion Complete ---")

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(
        description="Batch convert file encodings for a C++ project.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    
    parser.add_argument(
        '--recursive', '-r', 
        action='store_true', 
        default=False,
        help="Search files in subdirectories recursively (default is off)."
    )
    
    parser.add_argument(
        '--root', 
        type=str, 
        default='.', 
        help="Project root directory to start the search (default is current directory)."
    )
    
    args = parser.parse_args()
    
    batch_convert_project(args.root, args.recursive)