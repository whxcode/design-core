#include "z-document/include/commit/z-commit.h"

#include "z-app/include/ZAppEvent.h"
#include "z-document/include/commit/z-history.h"
#include "z-document/include/layers/z-document.h"
#include "z-editor/include/selection/z-selection.h"

ZCommit::ZCommit(z_sp<ZDocument> document, z_sp<ZSelection> selection, ZAppEvent* appEvent)
    : zAppEvent(appEvent),
      zHistory(std::make_shared<ZHistory>()),
      zDocument(document),
      zSelection(selection) {
}

void ZCommit::commit(ZPatchHandler&& handler) {
    ZGuidArray guids;
    // 收集图层数据.
    if (handler) {
        guids = zSelection->getSelectedLayerGuids();

        handler();
    }

    auto patch = zDocument->commit();
    if (!patch.has_value()) {
        return;
    }

    auto currentSelectedGuids = zSelection->getSelectedLayerGuids();
    if (currentSelectedGuids != guids) {
        patch->zUndo.zSelectedLayers = guids;
        patch->zRedo.zSelectedLayers = currentSelectedGuids;
    }

    zDocument->closeCollector();
    zHistory->pushUndo(patch.value());
    zDocument->openCollector();

    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zDocChanged);
        zAppEvent->emit(ZAppEventType::zHistoryChanged);
    }
}

void ZCommit::commit() {
    commit(nullptr);
}

void ZCommit::undo() {
    const auto& undo = zHistory->popUndo();

    // 运行这样
    if (!undo.has_value()) {
        return;
    }

    zDocument->closeCollector();
    zDocument->mergePatches(undo->zUndo);
    zDocument->openCollector();

    zSelection->select(undo->zUndo.zSelectedLayers);

    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zDocChanged);
        zAppEvent->emit(ZAppEventType::zHistoryChanged);
    }
}

void ZCommit::redo() {
    const auto& redo = zHistory->popRedo();

    if (!redo.has_value()) {
        return;
    }

    zDocument->closeCollector();
    zDocument->mergePatches(redo->zRedo);
    zDocument->openCollector();

    zSelection->select(redo->zRedo.zSelectedLayers);

    if (zAppEvent) {
        zAppEvent->emit(ZAppEventType::zDocChanged);
        zAppEvent->emit(ZAppEventType::zHistoryChanged);
    }
}

bool ZCommit::canUndo() const {
    return zHistory && zHistory->canUndo();
}

bool ZCommit::canRedo() const {
    return zHistory && zHistory->canRedo();
}
