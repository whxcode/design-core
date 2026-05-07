#pragma once

#include <memory>
#include <unordered_map>

#include "z-document/include/commit/z-collector.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-document-change-sink.h"
// #include "z-document/include/layers/z-layerbase.h"
// #include "z-document/include/models/z-document-model.h"

class ZLayerBase;
class ZDocumentModel;
class ZPage;

class ZDocument : public ZComponent, public ZDocumentChangeSink {
public:
    ZDocument(z_sp<ZDocumentModel> model);

public:  // ZDocumentChangeSink 实现
    void onModelPropChanged(const ZModel* const model, const ZPropKey key,
                            const void* const oldValue, const void* const newValue) override;

    // 收集子节点变化
    void onAddChild(ZComponent* comp) override;

    // 收集子节点变化
    void onRemoveChild(ZComponent* comp) override;

public:
    void rebuildIndex();

    void openCollector();
    void closeCollector();
    std::optional<ZPatch> commit();
    void mergePatches(const ZPatches& patches);

public:
    void addChild(const z_sp<ZComponent>& comp) override;
    z_sp<ZPage> getActivePage();
    z_sp<ZComponent> findKey(ZGuid id) const;
    z_sp<ZComponent> findKey(ZUniqueId id) const;

    void setActivePage(const ZUniqueId id);

public:
    void setName() {};

private:
    void registerSubtree(const z_sp<ZComponent>& node);

private:
    size_t zActivePageId{0};
    std::unordered_map<ZUniqueId, z_sp<ZPage>> zPages{};
    std::unordered_map<ZUniqueId, z_sp<ZComponent>> zLayers{};
    std::unique_ptr<ZCollector> zCollector{nullptr};
};
