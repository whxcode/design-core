#include "z-document/include/commit/z-commit.h"

#include "z-app/include/ZAppEvent.h"
#include "z-document/include/commit/z-history.h"
#include "z-document/include/layers/z-document.h"

ZCommit::ZCommit(z_sp<ZDocument> document, ZAppEvent* appEvent)
    : zAppEvent(appEvent), zHistory(std::make_shared<ZHistory>()), zDocument(document) {
}

void ZCommit::commit() {
    const auto& patch = zDocument->commit();
    if (!patch.has_value()) {
        return;
    }

    zDocument->closeCollector();
    zHistory->pushUndo(patch.value());
    zDocument->openCollector();

    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zDocChanged);
        zAppEvent->emit(ZAppEventType::zHistoryChanged);
    }
}

void ZCommit::undo() {
    const auto& undo = zHistory->popUndo();

    // 禁止多层嵌套
    if (undo.has_value()) {
        zDocument->closeCollector();
        zDocument->mergePatches(undo->zUndo);
        zDocument->openCollector();
        if (zAppEvent) {
            zAppEvent->emit(ZAppEventType::zDocChanged);
            zAppEvent->emit(ZAppEventType::zHistoryChanged);
        }
    }

    // 运行这样
    if (!undo.has_value()) {
        return;
    }

    zDocument->closeCollector();
    zDocument->mergePatches(undo->zUndo);
    zDocument->openCollector();
    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zDocChanged);
        zAppEvent->emit(ZAppEventType::zHistoryChanged);
    }
}

void ZCommit::redo() {
    const auto& redo = zHistory->popRedo();

    if (redo.has_value()) {
        zDocument->closeCollector();
        zDocument->mergePatches(redo->zRedo);
        zDocument->openCollector();
        if (zAppEvent) {
            zAppEvent->emit(ZAppEventType::zDocChanged);
            zAppEvent->emit(ZAppEventType::zHistoryChanged);
        }
    }
}

bool ZCommit::canUndo() const {
    return zHistory && zHistory->canUndo();
}

bool ZCommit::canRedo() const {
    return zHistory && zHistory->canRedo();
}
