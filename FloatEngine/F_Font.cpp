#include "F_Font.h"
#include <set>
#include "FloatApi.h"



F_Font::F_Font() 
    : _rayfnt()
    , _temp_data()
    , _temp_path()
    , _codepoints()
    , _font_size(0)
    , _font_type() {
}

F_Font::F_Font(const F_Font& font) 
    : _rayfnt()
    , _temp_data(font._temp_data)
    , _temp_path(font._temp_path)
    , _codepoints(font._codepoints)
    , _font_size(font._font_size)
    , _font_type(font._font_type) {
    
    // 深拷贝字体 - 需要重新加载
    if (!_temp_data.empty() && !_codepoints.empty()) {
        ReloadFont();
    }
}

F_Font::F_Font(F_Font&& font) noexcept
    : _rayfnt(font._rayfnt)
    , _temp_data(std::move(font._temp_data))
    , _temp_path(std::move(font._temp_path))
    , _codepoints(std::move(font._codepoints))
    , _font_size(font._font_size)
    , _font_type(std::move(font._font_type)) {
    
    // 将源对象重置为空状态
    font._rayfnt = Font();
    font._font_size = 0;
}

F_Font& F_Font::operator=(const F_Font& font) {
    if (this != &font) {
        Cleanup();
        
        _temp_data = font._temp_data;
        _temp_path = font._temp_path;
        _codepoints = font._codepoints;
        _font_size = font._font_size;
        _font_type = font._font_type;
        
        // 深拷贝字体 - 需要重新加载
        if (!_temp_data.empty() && !_codepoints.empty()) {
            ReloadFont();
        }
    }
    return *this;
}

F_Font& F_Font::operator=(F_Font&& font) noexcept {
    if (this != &font) {
        Cleanup();
        
        _rayfnt = font._rayfnt;
        _temp_data = std::move(font._temp_data);
        _temp_path = std::move(font._temp_path);
        _codepoints = std::move(font._codepoints);
        _font_size = font._font_size;
        _font_type = std::move(font._font_type);
        
        // 重置源对象
        font._rayfnt = Font();
        font._font_size = 0;
    }
    return *this;
}

F_Font::F_Font(const Font& _rfont)
    : _rayfnt(_rfont)
    , _temp_data()
    , _temp_path()
    , _codepoints()
    , _font_size(0)
    , _font_type() {
    
    ExtractCodepointsFromFont(_rfont);
}

F_Font::~F_Font() {
    Cleanup();
}

Font F_Font::to_raylib_font() const {
    return _rayfnt;
}

bool F_Font::Load(const char* filename, const char* type, int font_size, int* codepoints, int codepoints_count) {
    Cleanup();
    
    int data_size = 0;
    unsigned char* file_data = LoadFileData(filename, &data_size);
    if (!file_data || data_size == 0) {
        DEBUG_LOG(LOG_ERROR, "F_Font::Load: Failed to load file data", false);
        return false;
    }
    
    // 保存数据到vector
    _temp_data.assign(file_data, file_data + data_size);
    UnloadFileData(file_data);
    
    _font_size = font_size;
    _font_type = type ? type : ".ttf";
    _temp_path = filename;
    
    // 设置码点
    _codepoints.clear();
    if (codepoints != nullptr && codepoints_count > 0) {
        _codepoints.assign(codepoints, codepoints + codepoints_count);
    } else {
        // 如果没有提供码点，则加载常用码点
        int count = 0;
        auto cps_chinese = floatapi_font::LoadCommonCodepoints(count);
        _codepoints.assign(cps_chinese.get(), cps_chinese.get() + count);
    }
    
    // 加载字体
    return ReloadFont();
}

bool F_Font::LoadFromData(const unsigned char* data, int data_size, const char* type, int font_size, int* codepoints, int codepoints_count) {
    Cleanup();
    
    if (!data || data_size == 0) {
        DEBUG_LOG(LOG_ERROR, "F_Font::LoadFromData: Invalid data", false);
        return false;
    }
    
    _temp_data.assign(data, data + data_size);
    _font_size = font_size;
    _font_type = type ? type : ".ttf";
    
    // 设置码点
    _codepoints.clear();
    if (codepoints != nullptr && codepoints_count > 0) {
        _codepoints.assign(codepoints, codepoints + codepoints_count);
    }
    
    // 加载字体
    return ReloadFont();
}

void F_Font::Unload() {
    Cleanup();
}

bool F_Font::ReloadWithNewCodepoints(const std::vector<int>& new_codepoints) {
    if (new_codepoints.empty()) {
        DEBUG_LOG(LOG_WARNING, "F_Font::ReloadWithNewCodepoints: new_codepoints is empty", false);
        return false;
    }
    
    // 添加新码点
    for (int codepoint : new_codepoints) {
        if (std::find(_codepoints.begin(), _codepoints.end(), codepoint) == _codepoints.end()) {
            _codepoints.push_back(codepoint);
        }
    }
    
    // 排序和去重
    std::sort(_codepoints.begin(), _codepoints.end());
    _codepoints.erase(std::unique(_codepoints.begin(), _codepoints.end()), _codepoints.end());
    
    // 重新加载字体
    return ReloadFont();
}

bool F_Font::CheckAndAddCodepoints(const std::string& text) {
    if (text.empty()) {
        return false;
    }
    
    std::vector<int> text_codepoints = floatapi_font::ExtractCodepointsFromText(text);
    bool has_new_codepoints = false;
    
    for (int codepoint : text_codepoints) {
        if (floatapi_font::IsValidUnicodeCodepoint(codepoint)) {
            if (std::find(_codepoints.begin(), _codepoints.end(), codepoint) == _codepoints.end()) {
                _codepoints.push_back(codepoint);
                has_new_codepoints = true;
            }
        }
    }
    
    if (has_new_codepoints) {
        // 排序和去重
        std::sort(_codepoints.begin(), _codepoints.end());
        _codepoints.erase(std::unique(_codepoints.begin(), _codepoints.end()), _codepoints.end());
        
        // 重新加载字体
        return ReloadFont();
    }
    
    return false;
}

bool F_Font::ContainsText(const std::string& text) const {
    std::vector<int> text_codepoints = floatapi_font::ExtractCodepointsFromText(text);
    
    for (int codepoint : text_codepoints) {
        if (std::find(_codepoints.begin(), _codepoints.end(), codepoint) == _codepoints.end()) {
            return false;
        }
    }
    return true;
}

bool F_Font::ReloadFont() {
    if (_temp_data.empty() || _codepoints.empty()) {
        DEBUG_LOG(LOG_WARNING, "F_Font::ReloadFont: No data or codepoints available", false);
        return false;
    }
    
    // 卸载旧字体
    if (_rayfnt.texture.id != 0) {
        UnloadFont(_rayfnt);
    }
    
    // 加载新字体
    _rayfnt = LoadFontFromMemory(_font_type.c_str(), _temp_data.data(), 
                                static_cast<int>(_temp_data.size()), 
                                _font_size, _codepoints.data(), 
                                static_cast<int>(_codepoints.size()));
    
    if (_rayfnt.texture.id == 0) {
        DEBUG_LOG(LOG_ERROR, "F_Font::ReloadFont: Failed to reload font", false);
        return false;
    }
    
    return true;
}

void F_Font::Cleanup() {
    if (_rayfnt.texture.id != 0) {
        UnloadFont(_rayfnt);
        _rayfnt = Font();
    }
    _temp_data.clear();
    _temp_path.clear();
    _codepoints.clear();
    _font_size = 0;
    _font_type.clear();
}

void F_Font::ExtractCodepointsFromFont(const Font& font) {
    _codepoints.clear();
    if (font.glyphCount > 0) {
        _codepoints.reserve(font.glyphCount);
        for (int i = 0; i < font.glyphCount; i++) {
            _codepoints.push_back(font.glyphs[i].value);
        }
    } else {
        // 如果没有glyphs，则加载常用码点
        int count = 0;
        auto cps_chinese = floatapi_font::LoadCommonCodepoints(count);
        _codepoints.assign(cps_chinese.get(), cps_chinese.get() + count);
    }
}

// 全局默认字体
Font f_default_font;

namespace floatapi_font {
    void InitDefaultFont() {
        f_default_font = LoadChineseFontDefault();
    }
    
    Font LoadFontRaylib(const char* filename, const char* type, int font_size, int* codepoints, int codepoints_count) {
        int data_size = 0;
        unsigned char* data = LoadFileData(filename, &data_size);
        if (!data || data_size == 0) {
            return Font();
        }
        
        Font temp = LoadFontFromMemory(type, data, data_size, font_size, codepoints, codepoints_count);
        UnloadFileData(data);
        return temp;
    }
    
    Font LoadFontRaylibFromData(const unsigned char* data, int data_size, const char* type, int font_size, int* codepoints, int codepoints_count) {
        return LoadFontFromMemory(type, data, data_size, font_size, codepoints, codepoints_count);
    }
    
    Font LoadChineseFontDefault() {
        int count = 0;
        auto cps_chinese = LoadCommonCodepoints(count);
        return LoadFontRaylib("C:\\windows\\fonts\\simhei.ttf", ".ttf", 30, cps_chinese.get(), count);
    }
    
    std::unique_ptr<int[]> LoadCommonCodepoints(int& count, const std::string& other) {
        std::vector<int> codepoints;
        
        // 定义要加载的Unicode范围
        std::vector<std::pair<int, int>> ranges = {
            {0x0000, 0x007F},   // 基本拉丁字母和数字
            {0x4E00, 0x9FFF},   // 汉字
            {0x3040, 0x309F},   // 平假名
            {0x30A0, 0x30FF},   // 片假名
            {0xFF00, 0xFFEF},   // 全角和半角字符
            {0x2000, 0x206F},   // 标点符号
            {0x2200, 0x22FF},   // 数学运算符
            {0x2190, 0x21FF},   // 箭头符号
            {0x1F600, 0x1F64F}, // 表情符号
            {0x1F300, 0x1F5FF}  // 符号和图形
        };
        
        for (const auto& range : ranges) {
            for (int codepoint = range.first; codepoint <= range.second; codepoint++) {
                if (IsValidUnicodeCodepoint(codepoint)) {
                    codepoints.push_back(codepoint);
                }
            }
        }
        
        if (!other.empty()) {
            int count_other = 0;
            int* cp_other = LoadCodepoints(other.c_str(), &count_other);
            if (cp_other && count_other > 0) {
                int total_count = 0;
                auto new_cp = ConcatenateCodepoints(codepoints.data(), static_cast<int>(codepoints.size()), 
                                                   cp_other, count_other, total_count);
                UnloadCodepoints(cp_other);
                
                // 更新codepoints
                codepoints.assign(new_cp.get(), new_cp.get() + total_count);
                count = total_count;
            }
        } else {
            count = static_cast<int>(codepoints.size());
        }
        
        // 使用unique_ptr管理内存
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        
        return result;
    }
    
    std::unique_ptr<int[]> LoadCodepointsFromRange(int start, int end, int& count) {
        std::vector<int> codepoints;
        for (int codepoint = start; codepoint <= end; codepoint++) {
            if (IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }
        count = static_cast<int>(codepoints.size());
        
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadChineseCodepoints(int& count) {
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
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadJapaneseCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // 平假名：U+3040 到 U+309F
        for (int codepoint = 0x3040; codepoint <= 0x309F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        // 片假名：U+30A0 到 U+30FF
        for (int codepoint = 0x30A0; codepoint <= 0x30FF; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadFrenchCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // 法语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F）
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadGermanCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // 德语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F）
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        // 包括德语特有字符：例如 U+00DF（ß）
        codepoints.push_back(0x00DF);  // ß
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadSpanishCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // 西班牙语的字符集范围：拉丁字母扩展区（U+00C0 - U+024F）
        for (int codepoint = 0x00C0; codepoint <= 0x024F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        // 西班牙语的重音字符
        codepoints.push_back(0x00E9);  // é
        codepoints.push_back(0x00F1);  // ñ
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadAsciiCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // ASCII 字符集范围：U+0000 到 U+007F
        for (int codepoint = 0x0000; codepoint <= 0x007F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> LoadEmojiCodepoints(int& count) {
        std::vector<int> codepoints;
        
        // Emoji 表情符号的 Unicode 范围（U+1F600 - U+1F64F）
        for (int codepoint = 0x1F600; codepoint <= 0x1F64F; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        // U+1F300 到 U+1F5FF（符号和图形）
        for (int codepoint = 0x1F300; codepoint <= 0x1F5FF; codepoint++) {
            codepoints.push_back(codepoint);
        }
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    bool IsValidUnicodeCodepoint(int codepoint) {
        return !((codepoint >= 0x0000 && codepoint <= 0x001F) || // 控制字符
                (codepoint >= 0x007F && codepoint <= 0x009F) || // 控制字符
                (codepoint >= 0xFDD0 && codepoint <= 0xFDEF) || // 私有区
                (codepoint >= 0xFFFE && codepoint <= 0xFFFF));  // 非字符
    }
    
    std::unique_ptr<int[]> LoadUnicodeCodepoints(int& count, bool filter) {
        std::vector<int> codepoints;
        
        // 遍历 Unicode 范围：U+0000 到 U+FFFF
        for (int codepoint = 0x0000; codepoint <= 0xFFFF; codepoint++) {
            // 过滤掉无效的字符（控制字符、私有区等）
            if (!filter || IsValidUnicodeCodepoint(codepoint)) {
                codepoints.push_back(codepoint);
            }
        }
        
        count = static_cast<int>(codepoints.size());
        auto result = std::make_unique<int[]>(count);
        std::copy(codepoints.begin(), codepoints.end(), result.get());
        return result;
    }
    
    std::unique_ptr<int[]> ConcatenateCodepoints(int* codepoints1, int count1, int* codepoints2, int count2, int& count) {
        count = count1 + count2;
        auto result = std::make_unique<int[]>(count);
        
        // 复制第一个数组
        if (codepoints1 && count1 > 0) {
            std::copy(codepoints1, codepoints1 + count1, result.get());
        }
        
        // 复制第二个数组
        if (codepoints2 && count2 > 0) {
            std::copy(codepoints2, codepoints2 + count2, result.get() + count1);
        }
        
        return result;
    }
    
    std::vector<int> ExtractCodepointsFromText(const std::string& text) {
        std::vector<int> codepoints;
        if (text.empty()) {
            return codepoints;
        }
        
        // 使用raylib的函数提取码点
        int count = 0;
        int* cps = LoadCodepoints(text.c_str(), &count);
        if (cps && count > 0) {
            codepoints.assign(cps, cps + count);
            UnloadCodepoints(cps);
        }
        
        return codepoints;
    }
}