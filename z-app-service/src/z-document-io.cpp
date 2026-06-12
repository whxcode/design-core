#include "z-app-service/include/z-document-io.h"

#include <utility>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/z-model-type.h"
#include "z-kiwi/include/kiwi.h"
#include "z-kiwi/include/z-kiwi-reader.h"
#include "z-kiwi/include/z-kiwi-writer.h"

namespace {

void collectModels(const z_sp<ZComponent>& comp, ZModelArray& models) {
    if (!comp) {
        return;
    }

    auto model = comp->getModel();
    if (model) {
        models.push_back(model);
    }

    for (const auto& child : comp->getChildren<ZComponent>()) {
        collectModels(child, models);
    }
}

bool encodeModels(const ZModelArray& models, std::vector<uint8_t>& out) {
    kiwi::ByteBuffer bb;
    if (!ZKiwiWriter::encode(models, bb)) {
        return false;
    }

    out.assign(bb.data(), bb.data() + bb.size());
    return true;
}

bool appendDecodedModels(const std::vector<uint8_t>& document, ZModelArray& out) {
    if (document.empty()) {
        return false;
    }

    kiwi::ByteBuffer bb(document.data(), document.size());
    auto models = ZKiwiReader::decode(bb);
    if (models.empty()) {
        return false;
    }

    out.insert(out.end(), models.begin(), models.end());
    return true;
}

bool hasPageModel(const ZModelArray& models) {
    for (const auto& model : models) {
        if (model && model->getType() == ZModelType::zPage) {
            return true;
        }
    }

    return false;
}

}  // namespace

ZDocumentPackage ZDocumentIO::ExportDocument(const ZDocument& document) {
    ZDocumentPackage package;

    ZModelArray documentModels;
    if (auto model = document.getModel()) {
        documentModels.push_back(model);
        package.name = model->getName();
    }
    encodeModels(documentModels, package.document);

    for (const auto& child : document.getChildren<ZComponent>()) {
        if (!child || child->getType() != ZModelType::zPage) {
            continue;
        }

        ZModelArray pageModels;
        collectModels(child, pageModels);

        ZDocumentPageFile page;
        page.id = child->getModel()->getId().toString();
        if (encodeModels(pageModels, page.document)) {
            package.pages.push_back(std::move(page));
        }
    }

    return package;
}

ZDocumentIOResult ZDocumentIO::ReadDocument(const ZDocumentPackage& package) {
    ZDocumentIOResult result;

    if (!appendDecodedModels(package.document, result.models)) {
        result.message = "document.kiwi is invalid";
        return result;
    }

    if (package.pages.empty()) {
        result.message = "page kiwi is empty";
        return result;
    }

    for (const auto& page : package.pages) {
        if (!appendDecodedModels(page.document, result.models)) {
            result.message = "page kiwi is invalid";
            result.models.clear();
            return result;
        }
    }

    result.success = true;
    return result;
}

ZDocumentIOResult ZDocumentIO::ReadLegacyDocument(const std::vector<uint8_t>& document) {
    ZDocumentIOResult result;

    if (!appendDecodedModels(document, result.models)) {
        result.message = "document kiwi is invalid";
        return result;
    }

    if (!hasPageModel(result.models)) {
        result.message = "page kiwi is empty";
        result.models.clear();
        return result;
    }

    result.success = true;
    return result;
}
