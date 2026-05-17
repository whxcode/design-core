#include <memory>

#include "z-document/include/commit/z-patch.h"
#include "z-tools/include/z-type.h"
#pragma pragma

class ZAppEvent;
class ZHistory;
class ZDocument;
class ZSelection;

class ZCommit {
public:
    ZCommit(z_sp<ZDocument> document, z_sp<ZSelection> selection, ZAppEvent* appEvent = nullptr);

public:
    void commit();
    void commit(ZPatchHandler&& handler);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

private:
    ZAppEvent* zAppEvent{nullptr};
    z_sp<ZHistory> zHistory{nullptr};
    z_sp<ZDocument> zDocument{nullptr};
    z_sp<ZSelection> zSelection{nullptr};
};
