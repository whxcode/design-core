#pragma once

#include <emscripten/val.h>

#include <string>
#include <vector>

#include "z-app-service/include/z-document-io.h"

struct ZWasmDocumentPayload {
    bool success{false};
    bool legacy{false};
    std::string message;
    std::vector<uint8_t> legacyDocument;
    ZDocumentPackage package;
};

class ZWasmDocumentAdapter {
public:
    static emscripten::val ToValue(const ZDocumentPackage& package);
    static emscripten::val ToValue(const ZDocumentIOResult& result);
    static ZWasmDocumentPayload ToDocumentPayload(const emscripten::val& payload);
};
