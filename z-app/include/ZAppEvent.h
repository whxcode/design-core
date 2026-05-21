#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

enum class ZAppEventType : uint32_t {
    zNone = 0,
    zDocChanged = 1,
    zViewportChanged = 2,
    zHistoryChanged = 3,
    zHandlerChanged = 4,
    zHoverLayerChanged = 5,
    zSelectedLayerChanged = 6,
    zTraceChanged = 7,
    zEditorModeChanged = 8,
};

class ZAppEvent {
public:
    using Listener = std::function<void(ZAppEventType)>;

    size_t on(ZAppEventType type, Listener listener) {
        const auto id = ++zNextListenerId;
        zListeners[static_cast<uint32_t>(type)].push_back({id, std::move(listener)});
        return id;
    }

    void off(ZAppEventType type, size_t listenerId) {
        auto it = zListeners.find(static_cast<uint32_t>(type));
        if (it == zListeners.end()) {
            return;
        }

        auto& items = it->second;
        std::erase_if(items, [listenerId](const ListenerItem& item) {
            return item.id == listenerId;
        });
    }

    void emit(ZAppEventType type) {
        auto it = zListeners.find(static_cast<uint32_t>(type));
        if (it == zListeners.end()) {
            return;
        }

        const auto items = it->second;
        for (const auto& item : items) {
            if (item.listener) {
                item.listener(type);
            }
        }
    }

private:
    struct ListenerItem {
        size_t id{0};
        Listener listener{};
    };

    size_t zNextListenerId{0};
    std::unordered_map<uint32_t, std::vector<ListenerItem>> zListeners{};
};
