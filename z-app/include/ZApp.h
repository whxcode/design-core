#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "z-document/include/layers/z-document.h"
#include "z-engine/include/z-engine.h"
#include "z-window/include/ZWindow.h"

// #include "document/include/Document.h"
// #include "window/include/Window.h"

class ZWindow;
class ZDocument;

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

public:
    // 暴露给外部获取子模块的入口
    ZWindow& getWindow() const;
    ZDocument& getDocument() const;

public:
    void addImage(uintptr_t ptr, size_t size, float x = 0.0f, float y = 0.0f, float width = 0.0f,
                  float height = 0.0f);
    void clearImages();
    const std::vector<ZImagePayload>& getImages() const;
    void renderImages(IZEngine* engine) const;
    void requestRedraw();

private:
    ZApp();  // 构造函数私有化

private:
    std::unique_ptr<ZWindow> zWindow{nullptr};
    z_sp<ZDocument> zDocument{nullptr};
    std::vector<ZImagePayload> zImages;
};
