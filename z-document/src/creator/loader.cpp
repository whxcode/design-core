
#include "z-document/include/creator/loader.h"

#include <algorithm>
#include <deque>  // 有时 ranges::to 需要完整的容器声明
#include <functional>
#include <ranges>
#include <unordered_map>

#include "z-document/include/creator/create-layer.h"
#include "z-tools/include/z-assert.h"
#include "z-tools/include/z-guid.h"

z_sp<ZDocument> ZLoader::MakeDocument(ZModelArray&& models) {
    for (const auto& model : models) {
        ZCreatorLayer::Make(model);
    }

    auto view = models |  //
                std::views::transform([](auto model) {
                    return ZCreatorLayer::Make(model);
                }) |
                std::ranges::to<std::vector>();

    std::unordered_map<size_t, z_sp<ZComponent>> treeMap;
    std::vector<std::function<void()>> func{};

    z_sp<ZDocument> result{nullptr};

    std::for_each(view.begin(), view.end(),
                  [&treeMap, &func, &result](const z_sp<ZComponent>& layer) {
                      // printf("type[%d],id[%d]\n", layer->getType(), layer->getUnique());

                      Z_ASSERT(treeMap.find(layer->getUnique()) == treeMap.end(),
                               "error repeat unique id: " + std::to_string(layer->getUnique()));

                      treeMap[layer->getUnique()] = layer;

                      // printf("model(%d)\n", layer->getModel().get());

                      if (layer->getModel()->getId() == ZGuid::zDocumentID) {
                          result = layer->as<ZDocument>();
                      }

                      func.push_back([&treeMap, layer]() {
                          auto it = treeMap.find(layer->getParentUnique());

                          if (it != treeMap.end()) {
                              auto parent = it->second;
                              // 这里可以根据需要将 layer 添加到 parent 的子组件列表中
                              parent->addChild(layer);
                          }
                      });
                  });

    if (!result) {
        Z_ASSERT(false, "doc is nullptr\n");
        // result->rebuildIndex();
    }

    result->closeCollector();

    for (const auto& f : func) {
        f();
    }

    result->rebuildIndex();
    result->openCollector();

    return result;
}

ZLayerBaseArray ZLoader::MakeViews(ZModelArray&& models) {
    auto view = models |  //
                std::views::transform([](auto model) {
                    return ZCreatorLayer::Make(model);
                }) |
                std::ranges::to<std::vector>();

    std::unordered_map<size_t, z_sp<ZComponent>> treeMap;
    std::vector<std::function<void()>> func{};
    ZLayerBaseArray result;

    std::for_each(view.begin(), view.end(),
                  [&treeMap, &func, &result](const z_sp<ZComponent>& layer) {
                      // printf("type[%d],id[%d]\n", layer->getType(), layer->getUnique());

                      Z_ASSERT(treeMap.find(layer->getUnique()) == treeMap.end(),
                               "error repeat unique id: " + std::to_string(layer->getUnique()));

                      treeMap[layer->getUnique()] = layer;

                      func.push_back([&treeMap, &result, layer]() {
                          auto it = treeMap.find(layer->getParentUnique());

                          if (it != treeMap.end()) {
                              auto parent = it->second;
                              // 这里可以根据需要将 layer 添加到 parent 的子组件列表中
                              parent->addChild(layer);
                          } else {
                              result.push_back(layer->as<ZLayerBase>());
                          }
                      });
                  });

    for (const auto& f : func) {
        f();
    }

    return result;
}
