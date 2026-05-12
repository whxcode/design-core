#include "z-document/include/commit/z-patch-merger.h"

#include <vector>

#include "z-document/include/creator/create-layer.h"
#include "z-document/include/creator/create-model.h"
#include "z-document/include/creator/loader.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-document/include/z-model-type.h"
#include "z-tools/include/z-assert.h"

namespace {

z_sp<ZComponent> MakeComponent(const ZPatchProps& props) {
    const auto& typeIt = props.get<ZPropKey::zType>();
    const auto& idIt = props.get<ZPropKey::zId>();

    /*
      if (typeIt == props.end()) {
          Z_ASSERT(false, "未能找到将要添加的节点类型");
          return nullptr;
      }

      if (idIt == props.end()) {
          Z_ASSERT(false, "未能找到将要添加的节点 id");
          return nullptr;
      }
    */

    const auto model = ZCreatorModel::Make(idIt, typeIt);
    // model->setProps(props);
    return ZCreatorLayer::Make(model);
}

std::vector<z_sp<ZModel>> MakeModels(const std::vector<const ZPatchProps*>& newProps) {
    std::vector<z_sp<ZModel>> result{};
    result.reserve(newProps.size());

    for (const auto& props : newProps) {
        const auto typeIt = props->get<ZPropKey::zType>();
        const auto idIt = props->get<ZPropKey::zId>();
        const auto model = ZCreatorModel::Make(idIt, typeIt);

        model->merge(props->getProps());

        result.push_back(std::move(model));
    }

    return result;
}

}  // namespace

void ZPatchMerger::Merge(ZDocument& document, const ZPatches& patches) {
    std::vector<const ZPatchProps*> newProps;

    for (const auto& item : patches) {
        switch (item.zType) {
            case ZPatchType::zProps: {
                const auto comp = document.findKey(item.zId);
                if (!comp) {
                    continue;
                }

                comp->getModel()->setProps(item.zProps);
                break;
            }

            case ZPatchType::zRemove: {
                const auto comp = document.findKey(item.zId);
                if (!comp) {
                    continue;
                }

                const auto parent = document.findKey(comp->getModel()->getParentId());
                if (!parent) {
                    Z_ASSERT(false, "未能找到将要移除的父节点");
                    continue;
                }

                parent->removeChild(comp);
                break;
            }

            case ZPatchType::zAdd: {
                newProps.push_back(&item.zProps);
                break;
            }
        }
    }

    if (!newProps.empty()) {
        const auto& layers = ZLoader::MakeViews(MakeModels(newProps));
        for (auto& layer : layers) {
            auto& pid = layer->getModel()->getParentId();
            auto parent = document.findKey(pid);

            if (parent) {
                parent->addChild(layer);
            }
        }
    }
}
