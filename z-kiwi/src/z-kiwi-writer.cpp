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

void fillGuid(schema::Guid* g, const ZGuid& src) {
    g->set_low(static_cast<uint64_t>(src.zSessionId));
    g->set_high(static_cast<uint64_t>(src.zClientId));
}

void fillSize(schema::Size* s, const ZSize& src) {
    s->set_width(src.width());
    s->set_height(src.height());
}

void fillMatrix(schema::Matrix* m, const ZMatrix& src) {
    m->set_m0(src.get(0));
    m->set_m1(src.get(1));
    m->set_m2(src.get(2));
    m->set_m3(src.get(3));
    m->set_m4(src.get(4));
    m->set_m5(src.get(5));
}

void fillPoint(schema::Point* p, const ZPoint& src) {
    p->set_x(src.x());
    p->set_y(src.y());
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

void fillModelNode(kiwi::MemoryPool& pool, schema::ModelNode& node, const z_sp<ZModel>& model) {
    auto* id = pool.allocate<schema::Guid>();
    fillGuid(id, model->getId());
    node.set_id(id);

    node.set_type(static_cast<schema::ModelType>(static_cast<int>(model->getType())));

    auto* pid = pool.allocate<schema::Guid>();
    fillGuid(pid, model->getParentId());
    node.set_parentId(pid);

    node.set_name(pool.string(model->getName().c_str()));

    auto layer = std::dynamic_pointer_cast<ZLayerModel>(model);
    if (!layer) return;

    auto* sz = pool.allocate<schema::Size>();
    fillSize(sz, layer->getSize());
    node.set_size(sz);

    auto* mat = pool.allocate<schema::Matrix>();
    fillMatrix(mat, layer->getTransform());
    node.set_transform(mat);

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

            auto* cf = pool.allocate<schema::Point>();
            fillPoint(cf, sp.curveFrom);
            ptArr[j].set_curveFrom(cf);

            auto* ct = pool.allocate<schema::Point>();
            fillPoint(ct, sp.curveTo);
            ptArr[j].set_curveTo(ct);

            auto* pt = pool.allocate<schema::Point>();
            fillPoint(pt, sp.point);
            ptArr[j].set_point(pt);

            ptArr[j].set_hasCurveFrom(sp.hasCurveFrom);
            ptArr[j].set_hasCurveTo(sp.hasCurveTo);
            ptArr[j].set_fixed(sp.fixed);
            ptArr[j].set_curveMode(static_cast<uint32_t>(static_cast<int>(sp.curveMode)));
        }
    }
}

}  // namespace

bool ZKiwiWriter::encode(const ZModelArray& models, kiwi::ByteBuffer& bb) {
    kiwi::MemoryPool pool;
    schema::DocumentFile doc;

    doc.set_version(1);
    auto& children = doc.set_children(pool, (uint32_t)models.size());

    for (uint32_t i = 0; i < models.size(); i++) {
        fillModelNode(pool, children[i], models[i]);
    }

    return doc.encode(bb);
}
