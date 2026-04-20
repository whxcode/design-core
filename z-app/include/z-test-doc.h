#pragma once
#include <memory>

#include "z-document/include/creator/create-model.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-type.h"

class ZTestDoc {
public:
    static ZModelArray MakeDoc() {
        auto docModel = ZCreatorModel::Make<ZDocumentModel>(ZModelType::zDocument);
        auto pageModel = ZCreatorModel::Make<ZLayerModel>(ZModelType::zPage);
        auto rect1 = ZCreatorModel::Make<ZLayerModel>(ZModelType::zRectangle);

        rect1->setParentId(pageModel->getParentId());
        rect1->setSize({100.f, 100.f});

        pageModel->setParentId(docModel->getParentId());

        return {docModel, pageModel, rect1};
    };
};
