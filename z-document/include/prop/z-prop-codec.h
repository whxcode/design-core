#pragma once

#include <any>
#include <functional>

#include "z-document/include/commit/z-patch.h"
#include "z-document/include/layers/z-component.h"
#include "z-document/include/prop/z-prop-key.h"
#include "z-tools/include/z-type.h"

class ZModel;
class ZComponent;

class ZPropCodec {
    using Fn = std::function<void(z_sp<ZComponent> comp)>;

public:
    static ZPatchProps MakeProps(const z_sp<ZModel>& model);

    static void DeepBST(std::vector<z_sp<ZComponent>> comps, const Fn& fn);
    static void DeepBST(ZComponent* comp, const Fn& fn);
};
