#include "z-editor/include/command/z-command.h"

#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/layers/z-layerbase.h"
#include "z-editor/include/selection/z-selection.h"
#include "z-editor/include/z-editor-context.h"
#include "z-editor/include/z-editor-mode.h"
#include "z-tools/include/z-assert.h"

ZCommand::ZCommand(ZEditorContext* context) : zContext(context) {
    Z_ASSERT(zContext != nullptr, "ZCommand requires ZEditorContext");
    Z_ASSERT(zContext->getHandle() != nullptr, "ZCommand requires ZUIHandle");
    Z_ASSERT(zContext->getEditorMode() != nullptr, "ZCommand requires ZEditorMode");
    Z_ASSERT(zContext->getSelection() != nullptr, "ZCommand requires ZSelection");
    Z_ASSERT(zContext->getCommit() != nullptr, "ZCommand requires ZCommit");
}

bool ZCommand::canExecute(const ZCommandType type) const {
    switch (type) {
        case ZCommandType::zSwitchToCommonHandler:
        case ZCommandType::zDrawRectangle:
        case ZCommandType::zDrawEllipse:
        case ZCommandType::zDrawVector:
            return true;
        case ZCommandType::zUndoDocumentHistory:
            return zContext->getCommit()->canUndo();
        case ZCommandType::zRedoDocumentHistory:
            return zContext->getCommit()->canRedo();
        case ZCommandType::zDeleteSelectedLayer:
            return canDeleteSelectedLayer();
        case ZCommandType::zCancelCurrentInteraction:
            return false;
        case ZCommandType::zDownloadFile:
            return true;
    }
}

void ZCommand::execute(const ZCommandType type) {
    if (!canExecute(type)) {
        return;
    }

    switch (type) {
        case ZCommandType::zEscape:
            escape();
            return;
        case ZCommandType::zSwitchToCommonHandler:
            zContext->getEditorMode()->setMode(ZEditorModeType::zCursor);
            return;
        case ZCommandType::zDrawRectangle:
            zContext->getEditorMode()->setMode(ZEditorModeType::zDrawRectangle);
            return;
        case ZCommandType::zDrawEllipse:
            zContext->getEditorMode()->setMode(ZEditorModeType::zDrawOval);
            return;
        case ZCommandType::zDrawVector:
            zContext->getEditorMode()->setMode(ZEditorModeType::zDrawVector);
            return;
        case ZCommandType::zUndoDocumentHistory:
            zContext->getCommit()->undo();
            return;
        case ZCommandType::zRedoDocumentHistory:
            zContext->getCommit()->redo();
            return;
        case ZCommandType::zDeleteSelectedLayer:
            deleteSelectedLayer();
            return;
        case ZCommandType::zCancelCurrentInteraction:
            return;
        case ZCommandType::zDownloadFile:
            return;
    }
}

bool ZCommand::canDeleteSelectedLayer() const {
    const auto* selection = zContext->getSelection();

    for (const auto& layer : selection->getSelectedLayers()) {
        if (layer->getParent()) {
            return true;
        }
    }

    return false;
}

void ZCommand::escape() {
    const auto& mode = zContext->getEditorMode();

    if (mode->getMode() != ZEditorModeType::zCursor) {
        mode->setMode(ZEditorModeType::zCursor);
        return;
    }

    zContext->getSelection()->clear();
}

void ZCommand::deleteSelectedLayer() {
    auto selection = zContext->getSelection();
    auto commit = zContext->getCommit();

    commit->commit([&]() {
        zContext->getDocument()->removeLayers(selection->getSelectedLayers());

        selection->clear();
    });
}
