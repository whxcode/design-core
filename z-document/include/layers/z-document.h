#pragma once

#include <memory>
#include <unordered_map>

#include "z-document/include/commit/z-collector.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-page.h"
#include "z-document/include/models/z-document-change-sink.h"
#include "z-document/include/models/z-type.h"
// #include "z-document/include/layers/z-layerbase.h"
// #include "z-document/include/models/z-document-model.h"

class ZLayerBase;
class ZDocumentModel;
class ZPage;

class ZDocument : public ZComponent, public ZDocumentChangeSink {
public:
    ZDocument(z_sp<ZDocumentModel> model);

public:  // ZDocumentChangeSink 实现
    void onModelPropChanged(const ZGuid& id, const ZPropKey key, const PropValue& oldValue,
                            const PropValue& newValue) override;

    void onAddChild(const z_sp<ZComponent>& comp) override;

    void onRemoveChild(const z_sp<ZComponent>& comp) override;

    void removeLayers(const ZLayerBaseArray& layers);

public:
    void rebuildIndex();

    void openCollector();
    void closeCollector();
    std::optional<ZPatch> commit();
    void mergePatches(const ZPatches& patches);

public:
    void addChild(const z_sp<ZComponent>& comp) override;
    z_sp<ZPage> getActivePage();

    template <typename T>
        requires std::derived_from<T, ZComponent>
    z_sp<T> findKey(const ZGuid& id) const {
        auto comp = findKey(id.toNumber());
        if (comp != nullptr) {
            return comp->as<T>();
        }

        return nullptr;
    }

    z_sp<ZComponent> findKey(ZGuid id) const;

    z_sp<ZComponent> findKey(ZUniqueId id) const;

    void setActivePage(const ZUniqueId id);

public:
    void setName() {};

private:
    void registerSubtree(const z_sp<ZComponent>& node);
    void unregisterSubtree(const z_sp<ZComponent>& node);

private:
    size_t zActivePageId{0};
    std::unordered_map<ZUniqueId, z_sp<ZPage>> zPages{};
    std::unordered_map<ZUniqueId, z_sp<ZComponent>> zLayers{};
    std::unique_ptr<ZCollector> zCollector{nullptr};
};
