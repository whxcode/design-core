#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include "z-kiwi/include/z-kiwi-writer.h"

#include "z-document/include/models/z-document-model.h"
#include "z-document/include/models/z-layer-model.h"
#include "z-document/include/models/z-oval-model.h"
#include "z-document/include/models/z-rectangle-model.h"
#include "z-document/include/models/z-vector-model.h"
#include "z-kiwi/include/schema.h"
#include "z-matrix/include/z-matrix.h"
#include "z-matrix/include/z-size.h"

namespace {

schema::Size* getKiwiValue(kiwi::MemoryPool& pool, const ZSize& src) {
    auto* s = pool.allocate<schema::Size>();
    s->set_width(src.width());
    s->set_height(src.height());
    return s;
}

schema::Matrix* getKiwiValue(kiwi::MemoryPool& pool, const ZMatrix& src) {
    auto* m = pool.allocate<schema::Matrix>();
    m->set_m0(src.get(0));
    m->set_m1(src.get(1));
    m->set_m2(src.get(2));
    m->set_m3(src.get(3));
    m->set_m4(src.get(4));
    m->set_m5(src.get(5));
    return m;
}

schema::Point* getKiwiValue(kiwi::MemoryPool& pool, const ZPoint& src) {
    auto* p = pool.allocate<schema::Point>();
    p->set_x(src.x());
    p->set_y(src.y());
    return p;
}

void addPaints(kiwi::MemoryPool& pool, schema::ModelNode& node, const ZPaintArray& paints,
               bool isStroke) {
    if (!paints || paints->empty()) return;

    auto& arr = isStroke ? node.set_strokes(pool, (uint32_t)paints->size())
                         : node.set_fills(pool, (uint32_t)paints->size());
    for (uint32_t i = 0; i < paints->size(); i++) {
        const auto& src = (*paints)[i];
        arr[i].set_color(src.color);
        arr[i].set_opacity(src.opacity);
        arr[i].set_visible(src.visible);
        arr[i].set_strokeWidth(src.strokeWidth);
    }
}

void fillKiwiValue(kiwi::MemoryPool& pool, schema::ModelNode& node, const z_sp<ZModel>& model) {
    node.set_id(ZKiwiWriter::getKiwiValue(pool, model->getId()));
    node.set_type(static_cast<schema::ModelType>(static_cast<int>(model->getType())));
    node.set_parentId(ZKiwiWriter::getKiwiValue(pool, model->getParentId()));
    node.set_name(pool.string(model->getName().c_str()));

    auto layer = std::dynamic_pointer_cast<ZLayerModel>(model);
    if (!layer) return;

    node.set_size(getKiwiValue(pool, layer->getSize()));
    node.set_transform(getKiwiValue(pool, layer->getTransform()));

    addPaints(pool, node, layer->getFills(), false);
    addPaints(pool, node, layer->getStrokes(), true);

    auto oval = std::dynamic_pointer_cast<ZOvalModel>(model);
    if (oval) {
        node.set_startAngle(oval->getStartAngle());
        node.set_endAngle(oval->getEndAngle());
        node.set_innerRadius(oval->getInnerRadius());
    }

    auto vec = std::dynamic_pointer_cast<ZVectorModel>(model);
    if (!vec) return;

    node.set_windingRule(static_cast<uint32_t>(static_cast<int>(vec->getWindingRule())));

    const auto& paths = vec->getPaths();
    if (paths.empty()) return;

    auto& arr = node.set_paths(pool, (uint32_t)paths.size());
    for (uint32_t i = 0; i < paths.size(); i++) {
        const auto& src = paths[i];
        auto& dst = arr[i];
        dst.set_isClosed(src.isClosed);

        auto& ptArr = dst.set_points(pool, (uint32_t)src.points.size());
        for (uint32_t j = 0; j < src.points.size(); j++) {
            const auto& sp = src.points[j];
            ptArr[j].set_cornerRadius(sp.cornerRadius);

            ptArr[j].set_curveFrom(getKiwiValue(pool, sp.curveFrom));
            ptArr[j].set_curveTo(getKiwiValue(pool, sp.curveTo));
            ptArr[j].set_point(getKiwiValue(pool, sp.point));
            ptArr[j].set_hasCurveFrom(sp.hasCurveFrom);
            ptArr[j].set_hasCurveTo(sp.hasCurveTo);
            ptArr[j].set_fixed(sp.fixed);
            ptArr[j].set_curveMode(static_cast<uint32_t>(static_cast<int>(sp.curveMode)));
        }
    }
}

void fillKiwiValues(kiwi::MemoryPool& pool, kiwi::Array<schema::ModelNode>& arr,
                    const ZModelArray& models) {
    for (uint32_t i = 0; i < models.size(); i++) {
        fillKiwiValue(pool, arr[i], models[i]);
    }
}

}  // namespace

schema::Guid* ZKiwiWriter::getKiwiValue(kiwi::MemoryPool& pool, const ZGuid& src) {
    auto* g = pool.allocate<schema::Guid>();
    g->set_low(static_cast<uint64_t>(src.zSessionId));
    g->set_high(static_cast<uint64_t>(src.zClientId));
    return g;
}

kiwi::Array<schema::ModelNode> ZKiwiWriter::encode(kiwi::MemoryPool& pool,
                                                   const ZModelArray& models) {
    auto arr = pool.array<schema::ModelNode>((uint32_t)models.size());
    fillKiwiValues(pool, arr, models);
    return arr;
}

bool ZKiwiWriter::encode(const ZModelArray& models, kiwi::ByteBuffer& bb) {
    kiwi::MemoryPool pool;
    schema::DocumentFile doc;

    doc.set_version(1);
    if (!models.empty() && models.front()) {
        doc.set_id(getKiwiValue(pool, models.front()->getId()));
        doc.set_name(pool.string(models.front()->getName().c_str()));
    }

    auto& children = doc.set_children(pool, (uint32_t)models.size());
    fillKiwiValues(pool, children, models);

    return doc.encode(bb);
}
