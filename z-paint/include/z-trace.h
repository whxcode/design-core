#pragma once

#include <concepts>
#include <functional>
#include <utility>
#include <vector>

#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class IZEngine;
class ZAppEvent;
class ZEditorContext;
class ZHoverShape;
class ZSelectShape;
class ZShape;

class ZTrace {
public:
    using RedrawCallback = std::function<void()>;

    ZTrace(ZEditorContext* context, ZAppEvent* appEvent, RedrawCallback redrawCallback);
    ~ZTrace();

    template <typename T, typename... Args>
        requires std::derived_from<T, ZShape>
    z_sp<T> MakeShape(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    ZGuid addChild(z_sp<ZShape> shape);
    void removeChild(const ZGuid& id);
    void clear();
    void syncHoverLayer();
    void syncSelectedLayers();
    void render(IZEngine* engine, ZEditorContext* context);

private:
    ZEditorContext* zContext{nullptr};
    ZAppEvent* zAppEvent{nullptr};
    RedrawCallback zRedrawCallback{nullptr};
    size_t zHoverLayerChangedListenerId{0};
    size_t zSelectedLayerChangedListenerId{0};
    z_sp<ZHoverShape> zHoverShape{nullptr};
    z_sp<ZSelectShape> zSelectShape{nullptr};
    std::vector<z_sp<ZShape>> zChildren{};
};
