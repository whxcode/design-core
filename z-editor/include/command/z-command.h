#pragma once

#include "z-editor/include/command/z-command-type.h"

class ZEditorContext;

class ZCommand {
public:
    explicit ZCommand(ZEditorContext* context);

    bool canExecute(ZCommandType type) const;
    void execute(ZCommandType type);

private:
    bool canDeleteSelectedLayer() const;
    void deleteSelectedLayer();

private:
    ZEditorContext* zContext{nullptr};
};
