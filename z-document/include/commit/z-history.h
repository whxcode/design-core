#pragma once

#include <deque>
#include <optional>

#include "z-document/include/commit/z-patch.h"

class ZHistory {
public:
    bool canUndo() const {
        return !zRecords.empty() && zIndex > 0;
    }

    bool canRedo() const {
        return !zRecords.empty() && zIndex < zRecords.size();
    }

    void pushUndo(const ZPatch& patch) {
        // 已经 undo 过，需要重新清理对饮的 redo
        while (zRecords.size() > zIndex) {
            zRecords.pop_back();
        }

        // 丢弃之前的旧数据
        while (zRecords.size() >= zMaxIndex) {
            zRecords.pop_front();
            if (zIndex > 0) {
                --zIndex;
            }
        }

        zRecords.push_back(patch);
        zIndex = zRecords.size();
    }

    void pushRedo() {
        ++zIndex;
        // zRedo.push_back(patch);
    }

    std::optional<ZPatch> popUndo() {
        if (!canUndo()) {
            return std::nullopt;
        }

        auto patch = zRecords[zIndex - 1];
        --zIndex;
        return patch;
    }

    std::optional<ZPatch> popRedo() {
        if (!canRedo()) {
            return std::nullopt;
        }

        auto patch = zRecords[zIndex];
        ++zIndex;

        return patch;
    }

private:
    static constexpr size_t zMaxIndex{100};

    std::deque<ZPatch> zRecords{};
    size_t zIndex{0};

    // std::deque<ZPatch> zUndo;
    // std::deque<ZPatch> zRedo;
};
