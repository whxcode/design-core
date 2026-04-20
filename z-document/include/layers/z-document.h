#pragma once

#include <memory>

#include "z-document/include/layers/z-component.h"
#include "z-document/include/models/z-document-model.h"

class ZDocument : public ZComponent {
public:
    ZDocument(std::shared_ptr<ZDocumentModel> model);

public:
    void setName() {};
};
