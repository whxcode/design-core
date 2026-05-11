#pragma once

#include "z-document/include/commit/z-patch.h"

class ZDocument;

class ZPatchMerger {
public:
    static void Merge(ZDocument& document, const ZPatches& patches);
};
