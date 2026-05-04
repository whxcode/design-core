#include "z-tools/include/z-editor-theme.h"

namespace {

ZEditorThemeType gThemeType{ZEditorThemeType::zLight};

}  // namespace

void ZEditorTheme::SetTheme(const ZEditorThemeType type) {
    gThemeType = type;
}

ZEditorThemeType ZEditorTheme::GetTheme() {
    return gThemeType;
}

uint32_t ZEditorTheme::GetColor(const ZEditorThemeToken token) {
    const auto& current = CurrentTheme();
    const auto currentIt = current.find(token);
    if (currentIt != current.end()) {
        return currentIt->second;
    }

    const auto& light = LightTheme();
    const auto lightIt = light.find(token);
    if (lightIt != light.end()) {
        return lightIt->second;
    }

    return 0x000000;
}

const ZEditorTheme::TokenMap& ZEditorTheme::LightTheme() {
    static const TokenMap theme{
        {ZEditorThemeToken::zCanvasBackground, 0xF1F3F5},
        {ZEditorThemeToken::zHoverStroke, 0x2563EB},
        {ZEditorThemeToken::zSelectionStroke, 0x7C3AED},
    };

    return theme;
}

const ZEditorTheme::TokenMap& ZEditorTheme::DarkTheme() {
    static const TokenMap theme{
        {ZEditorThemeToken::zCanvasBackground, 0x18181B},
        {ZEditorThemeToken::zHoverStroke, 0x60A5FA},
    };

    return theme;
}

const ZEditorTheme::TokenMap& ZEditorTheme::CurrentTheme() {
    switch (gThemeType) {
        case ZEditorThemeType::zDark:
            return DarkTheme();
        case ZEditorThemeType::zLight:
            return LightTheme();
    }

    return LightTheme();
}
