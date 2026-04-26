#include "z-document/include/layers/z-page.h"

ZViewport& ZPage::getViewport() {
    return zViewport;
}

const ZViewport& ZPage::getViewport() const {
    return zViewport;
}
