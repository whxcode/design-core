#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "z-app/include/ZAppEvent.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/viewport/z-viewport.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-engine/include/z-engine.h"
#include "z-paint/include/z-trace.h"
#include "z-tools/include/z-editor-theme.h"
#include "z-window/include/ZWindow.h"

// #include "document/include/Document.h"
// #include "window/include/Window.h"

class ZCommit;
class ZDocument;
class ZWindow;

struct ZImagePayload {
    uintptr_t ptr{0};
    size_t size{0};
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
};

class ZApp {
public:
    // 单例模式，保证全局只有一个 App 实例
    static ZApp& Get() {
        static ZApp instance;
        return instance;
    }

public:
    // 禁止拷贝
    ZApp(const ZApp&) = delete;
    ZApp& operator=(const ZApp&) = delete;

public:
    // 核心生命周期函数
    void startup();
    void onHandleEvent();
    void onUIEvent(const ZUIEvent& event);

public:
    // 暴露给外部获取子模块的入口
    ZWindow& getWindow() const;
    ZDocument& getDocument() const;
    ViewportData getViewportData() const;
    ZHandlerType getHandlerType() const;
    void switchHandler(ZHandlerType type);
    ZAppEvent& getAppEvent() const;
    void setTheme(ZEditorThemeType type);

public:
    void addImage(uintptr_t ptr, size_t size, float x = 0.0f, float y = 0.0f, float width = 0.0f,
                  float height = 0.0f);
    void clearImages();
    const std::vector<ZImagePayload>& getImages() const;
    void requestRedraw();
    ZCommit& getCommit() const;

private:
    ZApp();  // 构造函数私有化

private:
    std::unique_ptr<ZAppEvent> zAppEvent{nullptr};
    std::unique_ptr<ZWindow> zWindow{nullptr};
    std::unique_ptr<ZEditorContext> zEditorContext{nullptr};
    std::unique_ptr<ZUIHandle> zUIHandle{nullptr};
    std::unique_ptr<ZSelection> zSelection{nullptr};
    std::unique_ptr<ZTrace> zTrace{nullptr};
    z_sp<ZDocument> zDocument{nullptr};
    z_sp<ZCommit> zCommit{nullptr};
    std::vector<ZImagePayload> zImages;
};
