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
        auto rect2 = ZCreatorModel::Make<ZLayerModel>(ZModelType::zRectangle);

        docModel->setName("Test Doc A");

        pageModel->setParentId(docModel->getId());

        rect1->setParentId(pageModel->getId());
        rect1->setSize({300.f, 300.f});
        rect1->setName("矩形 1");
        rect1->setTransform(ZMatrix::Translate(100.f, 100.f));

        rect2->setParentId(rect1->getId());
        rect2->setSize({100.f, 100.f});
        rect2->setName("矩形 2");

        rect2->setTransform(ZMatrix::Identity()
                                .preTranslate(100, 100)          //
                                .preScale(1.2, 1.5, 25.f, 25.f)  //(50.f, 50.f)    //
                                .preRotate(45.f, 25.f, 25.f)     //(50.f, 50.f)    //
        );

        return {docModel, pageModel, rect1, rect2};
    };
};
