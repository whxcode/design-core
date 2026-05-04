#pragma once

#include <cstdint>
#include <unordered_map>

enum class ZEditorThemeType : uint8_t {
    zLight = 0,
    zDark = 1,
};

enum class ZEditorThemeToken : uint8_t {
    zCanvasBackground = 0,
    zHoverStroke = 1,
    zSelectionStroke = 2,
};

class ZEditorTheme {
public:
    static void SetTheme(ZEditorThemeType type);
    static ZEditorThemeType GetTheme();
    static uint32_t GetColor(ZEditorThemeToken token);

private:
    using TokenMap = std::unordered_map<ZEditorThemeToken, uint32_t>;

    static const TokenMap& LightTheme();
    static const TokenMap& DarkTheme();
    static const TokenMap& CurrentTheme();
};
