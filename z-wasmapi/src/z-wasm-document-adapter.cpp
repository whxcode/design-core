#include "z-wasmapi/include/z-wasm-document-adapter.h"

#include <emscripten/bind.h>

using namespace emscripten;

namespace {

val toUint8Array(const std::vector<uint8_t>& bytes) {
    auto result = val::global("Uint8Array").new_(bytes.size());
    if (!bytes.empty()) {
        result.call<void>("set", typed_memory_view(bytes.size(), bytes.data()));
    }
    return result;
}

bool copyUint8Array(const val& src, std::vector<uint8_t>& out) {
    if (src.isNull() || src.isUndefined()) {
        return false;
    }

    auto byteArray = val::global("Uint8Array").new_(src);
    auto length = byteArray["length"].as<size_t>();
    if (length == 0) {
        return false;
    }

    out.resize(length);
    val bufferView = val(typed_memory_view(out.size(), out.data()));
    bufferView.call<void>("set", byteArray);
    return true;
}

}  // namespace

val ZWasmDocumentAdapter::ToValue(const ZDocumentPackage& package) {
    auto result = val::object();
    result.set("name", package.name);
    result.set("version", package.version);
    result.set("document", toUint8Array(package.document));

    auto pages = val::array();
    for (size_t i = 0; i < package.pages.size(); i++) {
        const auto& src = package.pages[i];
        auto page = val::object();
        page.set("id", src.id);
        page.set("document", toUint8Array(src.document));
        pages.set(i, page);
    }
    result.set("pages", pages);

    if (!package.patches.empty()) {
        result.set("patches", toUint8Array(package.patches));
    }

    return result;
}

val ZWasmDocumentAdapter::ToValue(const ZDocumentIOResult& result) {
    auto value = val::object();
    value.set("success", result.success);
    value.set("message", result.message);
    value.set("models", result.models.size());
    return value;
}

ZWasmDocumentPayload ZWasmDocumentAdapter::ToDocumentPayload(const val& payload) {
    ZWasmDocumentPayload result;

    if (payload.isNull() || payload.isUndefined()) {
        result.message = "document bytes is empty";
        return result;
    }

    auto documentBytes = payload["document"];
    if (documentBytes.isNull() || documentBytes.isUndefined()) {
        result.legacy = true;
        result.success = copyUint8Array(payload, result.legacyDocument);
        if (!result.success) {
            result.message = "document bytes is empty";
        }
        return result;
    }

    result.success = copyUint8Array(documentBytes, result.package.document);
    if (!result.success) {
        result.message = "document bytes is empty";
        return result;
    }

    auto name = payload["name"];
    if (!name.isNull() && !name.isUndefined()) {
        result.package.name = name.as<std::string>();
    }

    auto version = payload["version"];
    if (!version.isNull() && !version.isUndefined()) {
        result.package.version = version.as<uint32_t>();
    }

    auto patches = payload["patches"];
    if (!patches.isNull() && !patches.isUndefined()) {
        copyUint8Array(patches, result.package.patches);
    }

    auto pages = payload["pages"];
    if (!pages.isNull() && !pages.isUndefined()) {
        const auto length = pages["length"].as<size_t>();
        result.package.pages.reserve(length);
        for (size_t i = 0; i < length; i++) {
            auto pageValue = pages[i];
            auto pageBytes = pageValue["document"];
            if (pageBytes.isNull() || pageBytes.isUndefined()) {
                pageBytes = pageValue;
            }

            ZDocumentPageFile page;
            auto pageId = pageValue["id"];
            if (!pageId.isNull() && !pageId.isUndefined()) {
                page.id = pageId.as<std::string>();
            }

            if (!copyUint8Array(pageBytes, page.document)) {
                result.success = false;
                result.message = "page kiwi is invalid";
                return result;
            }
            result.package.pages.push_back(std::move(page));
        }
    }

    return result;
}
