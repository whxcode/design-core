#include "z-nodeapi/include/z-node-document-adapter.h"

#include <cstring>
#include <utility>
#include <vector>

namespace {

napi_value makeString(napi_env env, const std::string& value) {
    napi_value result;
    napi_create_string_utf8(env, value.c_str(), value.size(), &result);
    return result;
}

napi_value makeBuffer(napi_env env, const std::vector<uint8_t>& bytes) {
    void* data{nullptr};
    napi_value result;
    napi_create_buffer_copy(env, bytes.size(), bytes.data(), &data, &result);
    return result;
}

void setNamed(napi_env env, napi_value object, const char* name, napi_value value) {
    napi_set_named_property(env, object, name, value);
}

bool readBuffer(napi_env env, napi_value value, std::vector<uint8_t>& out) {
    bool isBuffer{false};
    napi_is_buffer(env, value, &isBuffer);
    if (!isBuffer) {
        return false;
    }

    void* data{nullptr};
    size_t size{0};
    napi_get_buffer_info(env, value, &data, &size);
    if (!data || size == 0) {
        return false;
    }

    auto* bytes = static_cast<uint8_t*>(data);
    out.assign(bytes, bytes + size);
    return true;
}

bool getNamed(napi_env env, napi_value object, const char* name, napi_value& result) {
    bool hasProperty{false};
    napi_has_named_property(env, object, name, &hasProperty);
    if (!hasProperty) {
        return false;
    }

    napi_get_named_property(env, object, name, &result);
    return true;
}

}  // namespace

napi_value ZNodeDocumentAdapter::ToValue(napi_env env, const ZDocumentIOResult& result) {
    napi_value value;
    napi_create_object(env, &value);

    napi_value success;
    napi_get_boolean(env, result.success, &success);
    setNamed(env, value, "success", success);

    setNamed(env, value, "message", makeString(env, result.message));

    napi_value models;
    napi_create_uint32(env, static_cast<uint32_t>(result.models.size()), &models);
    setNamed(env, value, "models", models);

    return value;
}

napi_value ZNodeDocumentAdapter::ToValue(napi_env env, const ZDocumentPackage& package) {
    napi_value value;
    napi_create_object(env, &value);

    setNamed(env, value, "name", makeString(env, package.name));

    napi_value version;
    napi_create_uint32(env, package.version, &version);
    setNamed(env, value, "version", version);

    setNamed(env, value, "document", makeBuffer(env, package.document));

    napi_value pages;
    napi_create_array_with_length(env, package.pages.size(), &pages);
    for (size_t i = 0; i < package.pages.size(); i++) {
        const auto& src = package.pages[i];
        napi_value page;
        napi_create_object(env, &page);
        setNamed(env, page, "id", makeString(env, src.id));
        setNamed(env, page, "document", makeBuffer(env, src.document));
        napi_set_element(env, pages, static_cast<uint32_t>(i), page);
    }
    setNamed(env, value, "pages", pages);

    if (!package.patches.empty()) {
        setNamed(env, value, "patches", makeBuffer(env, package.patches));
    }

    return value;
}

bool ZNodeDocumentAdapter::ToDocumentPackage(napi_env env, napi_value value,
                                             ZDocumentPackage& package, std::string& message) {
    napi_value document;
    if (!getNamed(env, value, "document", document) || !readBuffer(env, document, package.document)) {
        message = "document bytes is empty";
        return false;
    }

    napi_value pages;
    if (!getNamed(env, value, "pages", pages)) {
        message = "page kiwi is empty";
        return false;
    }

    uint32_t length{0};
    napi_get_array_length(env, pages, &length);
    package.pages.reserve(length);
    for (uint32_t i = 0; i < length; i++) {
        napi_value pageValue;
        napi_get_element(env, pages, i, &pageValue);

        napi_value pageDocument;
        if (!getNamed(env, pageValue, "document", pageDocument)) {
            pageDocument = pageValue;
        }

        ZDocumentPageFile page;
        if (!readBuffer(env, pageDocument, page.document)) {
            message = "page kiwi is invalid";
            return false;
        }

        napi_value pageId;
        if (getNamed(env, pageValue, "id", pageId)) {
            size_t size{0};
            napi_get_value_string_utf8(env, pageId, nullptr, 0, &size);
            std::vector<char> buffer(size + 1, '\0');
            napi_get_value_string_utf8(env, pageId, buffer.data(), buffer.size(), &size);
            page.id.assign(buffer.data(), size);
        }
        package.pages.push_back(std::move(page));
    }

    return true;
}
