#pragma once
#include <memory>

#include "document/include/Document.h"
#include "window/include/Window.h"

// #include "document/include/Document.h"
// #include "window/include/Window.h"

class ZWindow;
class ZDocument;

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
    // 暴露给外部获取子模块的入口
    ZWindow& window() const;

    ZDocument& document() const;

    // 核心生命周期函数
    void startup();

private:
    ZApp();  // 构造函数私有化

private:
    std::unique_ptr<ZWindow> zWindow{nullptr};
    std::unique_ptr<ZDocument> zDocument{nullptr};
};
