#include "F_Font.h"

F_Font::F_Font()
{
    _rayfnt = Font();
    _temp_data = new unsigned char[2];
}
F_Font::F_Font(const F_Font& font)
{
    this->_rayfnt = font._rayfnt;
    this->_temp_data = nullptr;
}
F_Font::F_Font(const Font& _rfont)
{
    this->_rayfnt = _rfont;
    this->_temp_data = new unsigned char[2];
}
Font F_Font::to_raylib_font()
{
    return _rayfnt;
}
void F_Font::Load(const char* filename, const char*type,int font_size,int*
                codepoints,int codepoints_count)
{
    int data_size;
    _temp_data = LoadFileData(filename, &data_size);
    _rayfnt = LoadFontFromMemory(type, _temp_data, data_size, font_size, codepoints, codepoints_count);
    UnloadFileData(_temp_data);
}

void F_Font::Unload()
{
    UnloadFont(_rayfnt);
    if (_temp_data != nullptr) {
        delete []_temp_data;
    }
}

Font f_default_font;

namespace floatapi_font {
    void InitDefaultFont()
    {
        f_default_font = LoadChineseFontDefault();
    }
    Font LoadFontRaylib(const char* filename, const char* type, int font_size, int*
        codepoints, int codepoints_count)
    {
        int data_size;
        unsigned char* data = LoadFileData(filename, &data_size);
        Font temp = LoadFontFromMemory(type, data, data_size, font_size, codepoints, codepoints_count);
        return temp;
    }
    Font LoadFontRaylibFromData(const unsigned char* data,
        int data_size, const char* type, int font_size, int*
        codepoints, int codepoints_count)
    {
        return  LoadFontFromMemory(type, data, data_size, font_size, codepoints, codepoints_count);

    }
    Font LoadChineseFontDefault()
    {
        int count;
        int *cps_chinese = LoadCommonCodepoints(count);
        int size;
        unsigned char* data = LoadFileData("C:\\windows\\fonts\\simhei.ttf", &size);
        Font temp = LoadFontRaylibFromData(data,size, ".ttc", 30, cps_chinese, count);
        MemFree(cps_chinese);
        return temp;
    }
    // 加载常用字符的 Unicode 码点
    int* LoadCommonCodepoints(int& count,std::string other) {
        std::vector<int> codepoints;
        // 基本拉丁字母和数字
        for (int codepoint = 0x0000; codepoint <= 0x007F; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }
        

        // 汉字
        for (int codepoint = 0x4E00; codepoint <= 0x9FFF; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 平假名
        for (int codepoint = 0x3040; codepoint <= 0x309F; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 片假名
        for (int codepoint = 0x30A0; codepoint <= 0x30FF; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 全角和半角字符
        for (int codepoint = 0xFF00; codepoint <= 0xFFEF; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 标点符号
        for (int codepoint = 0x2000; codepoint <= 0x206F; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 数学运算符
        for (int codepoint = 0x2200; codepoint <= 0x22FF; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }

        // 箭头符号
        for (int codepoint = 0x2190; codepoint <= 0x21FF; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }


       
        // 表情符号
        for (int codepoint = 0x1F600; codepoint <= 0x1F64F; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }
        int _count = count;
        if (!other.empty()) {
            int count_other;
            int* cp_other = LoadCodepoints(other.c_str(), &count_other);
            int* new_cp = ConcatenateCodepoints(codepoints.data(), count, cp_other, count_other, _count);
            delete[] cp_other;

            // 更新 codepoints
            codepoints.assign(new_cp, new_cp + _count);
            delete[] new_cp;
            count = _count;
        }

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        std::copy(codepoints.begin(), codepoints.end(), result);

        return result;  // 返回堆上的指针

    }
    int* LoadCodepointsFromRange(int start, int end, int& count)
    {
        std::vector<int> codepoints;
        for (int codepoint = start; codepoint <= end; codepoint++) {
                if (IsValidUnicodeCodepoint(codepoint))
            codepoints.push_back(codepoint);
        }
        count = codepoints.size();
        int* result = new int[count];
        for (int i = 0; i < count; i++)
            result[i] = codepoints[i];
        return result;  
    }
    // 返回中文字符集的所有 Unicode 码点，使用堆内存
    int* LoadChineseCodepoints(int& count) {
        std::vector<int> codepoints;
        // 汉字 Unicode 范围（CJK Unified Ideographs）
        // 这个范围通常包括 U+4E00 到 U+9FFF
        for (int codepoint = 0x0000; codepoint <= 0x007F; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint))
		    	codepoints.push_back(codepoint);
        }
        for (int codepoint = 0x4E00; codepoint <= 0x9FFF; codepoint++) {
            if(IsValidUnicodeCodepoint(codepoint))
                codepoints.push_back(codepoint);
        }
        count = codepoints.size();
        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }

    // 返回日文字符集的所有 Unicode 码点，使用堆内存
    int* LoadJapaneseCodepoints(int& count) {
        std::vector<int> codepoints;

        // 平假名：U+3040 到 U+309F
        for (int codepoint = 0x3040; codepoint <= 0x309F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        // 片假名：U+30A0 到 U+30FF
        for (int codepoint = 0x30A0; codepoint <= 0x30FF; codepoint++) {
            codepoints.push_back(codepoint);
        }

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }

    // 返回法语字符集的所有 Unicode 码点，使用堆内存
    int* LoadFrenchCodepoints(int& count) {
        std::vector<int> codepoints;

        // 法语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F）
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }

    // 返回德语字符集的所有 Unicode 码点，使用堆内存
    int* LoadGermanCodepoints(int& count) {
        std::vector<int> codepoints;

        // 德语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F），以及德语特有字符
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        // 包括德语特有字符：例如 U+00DF（ß）
        codepoints.push_back(0x00DF);  // ß

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }

    // 返回西班牙语字符集的所有 Unicode 码点，使用堆内存
    int* LoadSpanishCodepoints(int& count) {
        std::vector<int> codepoints;

        // 西班牙语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F）
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        // 西班牙语的重音字符：例如 U+00E9（é）
        codepoints.push_back(0x00E9);  // é
        codepoints.push_back(0x00F1);  // ñ

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }
    // 返回 ASCII 字符集的所有 Unicode 码点
    int* LoadAsciiCodepoints(int& count) {
        std::vector<int> codepoints;

        // ASCII 字符集范围：U+0000 到 U+007F
        for (int codepoint = 0x0000; codepoint <= 0x007F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }
    // 返回 Emoji 字符集的所有 Unicode 码点
    int* LoadEmojiCodepoints(int& count) {
        std::vector<int> codepoints;

        // Emoji 表情符号的 Unicode 范围（U+1F600 - U+1F64F）
        for (int codepoint = 0x1F600; codepoint <= 0x1F64F; codepoint++) {
            codepoints.push_back(codepoint);
        }

        // 你可以根据需要加入更多 Emoji 范围，例如：
        // U+1F300 到 U+1F5FF（符号和图形）
        for (int codepoint = 0x1F300; codepoint <= 0x1F5FF; codepoint++) {
            codepoints.push_back(codepoint);
        }

        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }// 判断是否是有效的 Unicode 字符
    bool IsValidUnicodeCodepoint(int codepoint) {
        // 忽略控制字符（U+0000 到 U+001F）以及私有区（U+E000 到 U+F8FF，U+F0000 到 U+FFFFD 等）
        return !((codepoint >= 0x0000 && codepoint <= 0x001F) || // 控制字符
            (codepoint >= 0x007F && codepoint <= 0x009F) || // 控制字符
            (codepoint >= 0xFDD0 && codepoint <= 0xFDEF) || // 私有区
            (codepoint >= 0xFFFE && codepoint <= 0xFFFF));  // 非字符
    }

    // 返回 0 到 0xFFFF 范围内所有有效的 Unicode 码点
    int* LoadUnicodeCodepoints(int& count,bool filter) {
        std::vector<int> codepoints;

        // 遍历 Unicode 范围：U+0000 到 U+FFFF
        for (int codepoint = 0x0000; codepoint <= 0xFFFF; codepoint++) {
            // 过滤掉无效的字符（控制字符、私有区等）
            if (IsValidUnicodeCodepoint(codepoint) || !filter) {
                codepoints.push_back(codepoint);
            }
        }

        // 获取有效字符的数量
        count = codepoints.size();

        // 使用 new 在堆上分配内存，复制 vector 的内容
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = codepoints[i];
        }

        return result;  // 返回堆上的指针
    }

    // 拼接多个 codepoints 数组
    int* ConcatenateCodepoints(int* codepoints1, int count1, int* codepoints2, int count2, int& count) {
        // 创建一个新的 vector 来存储拼接后的代码点
        std::vector<int> concatenated;

        // 将第一个 codepoints 数组添加到结果中
        for (int i = 0; i < count1; i++) {
            concatenated.push_back(codepoints1[i]);
        }

        // 将第二个 codepoints 数组添加到结果中
        for (int i = 0; i < count2; i++) {
            concatenated.push_back(codepoints2[i]);
        }

        // 返回拼接后的数组大小
        count = concatenated.size();

        // 使用 new 在堆上分配内存并复制数据
        int* result = new int[count];
        for (int i = 0; i < count; i++) {
            result[i] = concatenated[i];
        }

        return result;
    }
    
}