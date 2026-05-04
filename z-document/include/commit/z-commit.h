#include <memory>

#include "z-tools/include/z-type.h"
#pragma pragma

class ZAppEvent;
class ZHistory;
class ZDocument;

class ZCommit {
public:
    ZCommit(z_sp<ZDocument> document, ZAppEvent* appEvent = nullptr);

public:
    void commit();
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

private:
    ZAppEvent* zAppEvent{nullptr};
    z_sp<ZHistory> zHistory{nullptr};
    z_sp<ZDocument> zDocument{nullptr};
};
