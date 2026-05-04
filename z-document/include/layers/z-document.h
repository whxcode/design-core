#pragma once

#include <memory>
#include <unordered_map>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-model-change-sink.h"
// #include "z-document/include/layers/z-layerbase.h"
// #include "z-document/include/models/z-document-model.h"

class ZLayerBase;
class ZDocumentModel;
class ZPage;

class ZDocument : public ZComponent, public ZModelChangeSink {
public:
    ZDocument(z_sp<ZDocumentModel> model);

public:  // ZModelChangeSink 实现
    void onModelPropChanged(const ZModel* const model, const ZPropKey key,
                            const void* const oldValue, const void* const newValue) override;

public:
    void rebuildIndex();

public:
    void addChild(const z_sp<ZComponent>& comp) override;
    z_sp<ZPage> getActivePage();
    std::vector<z_sp<ZLayerBase>> getNonPageLayers() const;

    void setActivePage(const size_t id);

public:
    void setName() {};

private:
    void registerSubtree(const z_sp<ZComponent>& node);

private:
    size_t zActivePageId{0};
    std::unordered_map<size_t, z_sp<ZPage>> zPages{};
    std::unordered_map<size_t, z_sp<ZComponent>> zLayers{};
};
