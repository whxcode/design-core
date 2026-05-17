#pragma once

#include <vector>

#include "z-document/include/models/z-type.h"
#include "z-matrix/include/z-point.h"
#include "z-matrix/include/z-rect.h"
#include "z-tools/include/z-guid.h"
#include "z-tools/include/z-type.h"

class ZEditorContext;
class ZLayerBase;

class ZSelection {
public:
    explicit ZSelection(ZEditorContext* context);

    bool hitHover(const ZPoint& worldPoint);
    z_sp<ZLayerBase> getHoverLayer() const;
    ZGuid getHoverLayerId() const;
    const std::vector<z_sp<ZLayerBase>>& getSelectedLayers() const;
    ZRect getSelectedLayerWorldRect() const;

    void clear();

    void select(const ZGuidArray& guids);

    void select(const z_sp<ZLayerBase>& layer);
    void select(const ZLayerBaseArray& layers);

    void append(const z_sp<ZLayerBase>& layer);
    void append(const ZLayerBaseArray& layers);

    void selectInRect(const ZRect& worldRect);

    void refreshSelectedLayers();

    ZGuidArray getSelectedLayerGuids() const;

private:
    z_sp<ZLayerBase> hitTest(const ZPoint& worldPoint) const;
    void collectLayersInRect(const z_sp<ZLayerBase>& layer, const ZRect& worldRect,
                             std::vector<z_sp<ZLayerBase>>& result) const;

    void emitHoverChanged() const;
    void emitSelectedChanged() const;

    void notice() const;

    bool containsSelectedLayer(const ZGuid& id) const;

private:
    ZEditorContext* zContext{nullptr};
    ZGuid zHoverLayerId{};
    std::vector<z_sp<ZLayerBase>> zSelectedLayers{};
};
