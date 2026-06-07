#include "z-kiwi/include/z-kiwi-reader.h"

#include <cstdint>

#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-oval-model.h"
#include "z-document/include/models/z-rectangle-model.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-kiwi/include/schema.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"

namespace {

ZGuid getValue(const schema::Guid* g) {
    if (!g) return ZGuid();
    return ZGuid(static_cast<size_t>(*g->low()), static_cast<size_t>(*g->high()));
}

ZSize getValue(const schema::Size* s) {
    if (!s) return ZSize();
    return ZSize(*s->width(), *s->height());
}

ZMatrix getValue(const schema::Matrix* m) {
    if (!m) return ZMatrix();
    ZMatrix mat;
    mat.set(0, *m->m0());
    mat.set(1, *m->m1());
    mat.set(2, *m->m2());
    mat.set(3, *m->m3());
    mat.set(4, *m->m4());
    mat.set(5, *m->m5());
    return mat;
}

ZPaintArray getValue(const kiwi::Array<schema::Paint>* arr) {
    if (!arr || arr->size() == 0) return nullptr;

    auto paints = std::make_shared<std::vector<ZPaint>>();
    paints->reserve(arr->size());
    for (uint32_t i = 0; i < arr->size(); i++) {
        const auto& src = (*arr)[i];
        ZPaint p;
        p.color = src.color() ? static_cast<uint32_t>(*src.color()) : 0x000000FFu;
        p.opacity = src.opacity() ? *src.opacity() : 1.0f;
        p.visible = src.visible() ? *src.visible() : true;
        p.strokeWidth = src.strokeWidth() ? *src.strokeWidth() : 0.0f;
        paints->push_back(p);
    }
    return paints;
}

ZPathDataArray getValue(kiwi::MemoryPool& pool,
                           const kiwi::Array<schema::PathData>* arr) {
    ZPathDataArray result;
    if (!arr || arr->size() == 0) return result;

    result.reserve(arr->size());
    for (uint32_t i = 0; i < arr->size(); i++) {
        const auto& src = (*arr)[i];
        ZPathData pd;
        pd.isClosed = src.isClosed() ? *src.isClosed() : true;

        auto* ptArr = src.points();
        if (ptArr) {
            pd.points.reserve(ptArr->size());
            for (uint32_t j = 0; j < ptArr->size(); j++) {
                const auto& sp = (*ptArr)[j];
                ZPathPoint pp;
                pp.cornerRadius = sp.cornerRadius() ? *sp.cornerRadius() : 0.0f;
                if (auto* cf = sp.curveFrom()) {
                    pp.curveFrom = ZPoint(*cf->x(), *cf->y());
                }
                if (auto* ct = sp.curveTo()) {
                    pp.curveTo = ZPoint(*ct->x(), *ct->y());
                }
                if (auto* pt = sp.point()) {
                    pp.point = ZPoint(*pt->x(), *pt->y());
                }
                pp.hasCurveFrom = sp.hasCurveFrom() ? *sp.hasCurveFrom() : false;
                pp.hasCurveTo = sp.hasCurveTo() ? *sp.hasCurveTo() : false;
                pp.fixed = sp.fixed() ? *sp.fixed() : false;
                if (auto* cm = sp.curveMode()) {
                    pp.curveMode = static_cast<ZCurveMode>(*cm);
                }
                pd.points.push_back(pp);
            }
        }
        result.push_back(pd);
    }
    return result;
}

z_sp<ZModel> createModel(const schema::ModelNode& node) {
    auto id = getValue(node.id());
    auto rawType = static_cast<int>(node.type() ? *node.type() : schema::ModelType::Document);
    auto type = static_cast<ZModelType>(rawType);
    auto parentId = getValue(node.parentId());

    z_sp<ZModel> model;
    switch (type) {
        case ZModelType::zDocument:
            model = std::make_shared<ZDocumentModel>(id, ZModelType::zDocument);
            break;
        case ZModelType::zPage:
            model = std::make_shared<ZLayerModel>(id, ZModelType::zPage);
            break;
        case ZModelType::zRectangle:
            model = std::make_shared<ZRectangleModel>(id, ZModelType::zRectangle);
            break;
        case ZModelType::zOval:
            model = std::make_shared<ZOvalModel>(id, ZModelType::zOval);
            break;
        case ZModelType::zVector:
            model = std::make_shared<ZVectorModel>(id, ZModelType::zVector);
            break;
    }

    if (!model) return nullptr;

    model->setParentId(parentId);

    if (auto* name = node.name()) {
        model->setName(name->c_str());
    }

    auto layer = std::dynamic_pointer_cast<ZLayerModel>(model);
    if (!layer) return model;

    if (auto* s = node.size()) layer->setSize(getValue(s));
    if (auto* m = node.transform()) layer->setTransform(getValue(m));
    if (auto* f = node.fills()) layer->setFills(getValue(f));
    if (auto* s = node.strokes()) layer->setStrokes(getValue(s));

    auto oval = std::dynamic_pointer_cast<ZOvalModel>(model);
    if (oval) {
        if (auto* v = node.startAngle()) oval->setStartAngle(*v);
        if (auto* v = node.endAngle()) oval->setEndAngle(*v);
        if (auto* v = node.innerRadius()) oval->setInnerRadius(*v);
    }

    auto vec = std::dynamic_pointer_cast<ZVectorModel>(model);
    if (vec) {
        if (auto* w = node.windingRule())
            vec->setWindingRule(static_cast<ZWindingRule>(*w));
    }

    return model;
}

}  // namespace

ZModelArray ZKiwiReader::decode(kiwi::ByteBuffer& bb) {
    schema::DocumentFile doc;
    kiwi::MemoryPool pool;

    if (!doc.decode(bb, pool, nullptr)) {
        return {};
    }

    auto* children = doc.children();
    if (!children) return {};

    ZModelArray result;
    result.reserve(children->size());

    for (uint32_t i = 0; i < children->size(); i++) {
        const auto& child = (*children)[i];
        auto model = createModel(child);

        auto vec = std::dynamic_pointer_cast<ZVectorModel>(model);
        if (vec) {
            if (auto* paths = child.paths()) {
                vec->setPaths(getValue(pool, paths));
            }
        }

        if (model) result.push_back(model);
    }

    return result;
}
