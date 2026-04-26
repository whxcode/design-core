#pragma once

#include <memory>
#include <unordered_map>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-page.h"
// #include "z-document/include/layers/z-layerbase.h"
// #include "z-document/include/models/z-document-model.h"

class ZLayerBase;
class ZDocumentModel;
class ZPage;

class ZDocument : public ZComponent {
public:
    ZDocument(z_sp<ZDocumentModel> model);

public:
    void addChild(const z_sp<ZComponent>& comp) override;
    z_sp<ZPage> getActivePage();

    void setActivePage(const size_t id);

public:
    void setName() {};

private:
    size_t zActivePageId{0};
    std::unordered_map<size_t, z_sp<ZPage>> zPages{};
};
