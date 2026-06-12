#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "z-document/include/models/z-type.h"

class ZDocument;

struct ZDocumentPageFile {
    std::string id;
    std::vector<uint8_t> document;
};

struct ZDocumentPackage {
    std::string name;
    uint32_t version{1};
    std::vector<uint8_t> document;
    std::vector<ZDocumentPageFile> pages;
    std::vector<uint8_t> patches;
};

struct ZDocumentIOResult {
    bool success{false};
    std::string message;
    ZModelArray models;
};

class ZDocumentIO {
public:
    static ZDocumentPackage ExportDocument(const ZDocument& document);
    static ZDocumentIOResult ReadDocument(const ZDocumentPackage& package);
    static ZDocumentIOResult ReadLegacyDocument(const std::vector<uint8_t>& document);
};
