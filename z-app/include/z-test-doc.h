#pragma once
#include <iostream>
#include <memory>

#include "z-document/include/creator/create-model.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-rectangle-model.h"
#include "z-document/include/models/z-type.h"
#include "z-document/include/types/z-paint.h"

class ZTestDoc {
public:
    static ZModelArray MakeDoc() {
        auto docModel = ZCreatorModel::Make<ZDocumentModel>(ZModelType::zDocument);
        auto pageModel = ZCreatorModel::Make<ZLayerModel>(ZModelType::zPage);
        auto rect1 = ZCreatorModel::Make<ZRectangleModel>(ZModelType::zRectangle);
        auto rect2 = ZCreatorModel::Make<ZRectangleModel>(ZModelType::zRectangle);
        auto rect3 = ZCreatorModel::Make<ZRectangleModel>(ZModelType::zRectangle);

        docModel->setName("Test Doc A");

        pageModel->setParentId(docModel->getId());

        rect1->setParentId(pageModel->getId());
        rect1->setSize({240.f, 180.f});
        rect1->setName("矩形 1");
        {
            auto fills = std::make_shared<std::vector<ZPaint>>();
            fills->push_back({0x00ff00, 0.5f, true});  // 底色黑
            fills->push_back({0xff0000, 0.6f, true});  // 半透明红
            rect1->setFills(fills);

            auto strokes = std::make_shared<std::vector<ZPaint>>();
            strokes->push_back({0x000000, 1.0f, true, 2.0f});  // 黑色边框 2px
            rect1->setStrokes(strokes);
        }
        rect1->setTransform(ZMatrix::Translate(0, 0));

        rect2->setParentId(pageModel->getId());
        rect2->setSize({180.f, 120.f});
        rect2->setName("矩形 2");
        {
            auto fills = std::make_shared<std::vector<ZPaint>>();
            fills->push_back({0x16A34A, 1.0f, true});
            rect2->setFills(fills);

            auto strokes = std::make_shared<std::vector<ZPaint>>();
            strokes->push_back({0x000000, 1.0f, true, 3.0f});  // 3px 黑边框
            rect2->setStrokes(strokes);
        }
        rect2->setTransform(
            ZMatrix::Identity().preTranslate(420.f, 150.f).preRotate(12.f, 90.f, 60.f));

        rect3->setParentId(pageModel->getId());
        rect3->setSize({140.f, 220.f});
        rect3->setName("矩形 3");
        {
            auto fills = std::make_shared<std::vector<ZPaint>>();
            fills->push_back({0xF97316, 0.8f, true});
            rect3->setFills(fills);
        }
        rect3->setTransform(
            ZMatrix::Identity().preTranslate(260.f, 360.f).preRotate(-18.f, 70.f, 110.f));

        return {docModel, pageModel, rect1, rect2, rect3};
    };
};
