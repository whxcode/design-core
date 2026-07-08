#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "z-app/include/ZAppEvent.h"
#include "z-app-service/include/z-document-io.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/viewport/z-viewport.h"
#include "z-editor/include/command/z-command.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/ui-event/z-ui-handle.h"
#include "z-editor/include/z-editor-context.h"
#include "z-editor/include/z-editor-mode.h"
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
    ZEditorModeType getEditorModeType() const;
    void switchHandler(ZHandlerType type);
    ZAppEvent& getAppEvent() const;
    void setTheme(ZEditorThemeType type);

public:
    // 用户可以将使用 exportDocument 导出的文件再次打开
    void openDocument(const std::vector<uint8_t>& document);
    void openDocument(const ZDocumentPackage& document);
    ZDocumentIOResult openLegacyDocument(const std::vector<uint8_t>& document);
    ZDocumentIOResult openDocumentPackage(const ZDocumentPackage& document);
    // 打开测试文档
    void openDocument(const ZModelArray& documents);
    ZDocumentPackage exportDocumentPackage() const;

private:
    // 重载所有状态 ZApp::startup
    void flusDocument();

public:
    void addImage(uintptr_t ptr, size_t size, float x = 0.0f, float y = 0.0f, float width = 0.0f,
                  float height = 0.0f);
    void clearImages();
    const std::vector<ZImagePayload>& getImages() const;
    void requestRedraw();
    ZCommit& getCommit() const;
    ZCommand& getCommand() const;

private:
    ZApp();  // 构造函数私有化

private:
    std::unique_ptr<ZAppEvent> zAppEvent{nullptr};
    std::unique_ptr<ZWindow> zWindow{nullptr};
    std::unique_ptr<ZEditorContext> zEditorContext{nullptr};
    std::unique_ptr<ZUIHandle> zUIHandle{nullptr};
    z_sp<ZSelection> zSelection{nullptr};
    std::unique_ptr<ZTrace> zTrace{nullptr};
    z_sp<ZDocument> zDocument{nullptr};
    z_sp<ZCommit> zCommit{nullptr};
    std::unique_ptr<ZEditorMode> zEditorMode{nullptr};
    std::unique_ptr<ZCommand> zCommand{nullptr};
    std::vector<ZImagePayload> zImages;
};
