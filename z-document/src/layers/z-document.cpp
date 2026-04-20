
#include "z-document/include/layers/z-document.h"

#include <cstdio>
#include <iostream>

ZDocument::ZDocument(std::shared_ptr<ZDocumentModel> model) : ZComponent(model) {
}
