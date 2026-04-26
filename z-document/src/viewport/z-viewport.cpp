#include "z-document/include/viewport/z-viewport.h"

const ViewportData& ZViewport::data() const {
    return zData;
}

void ZViewport::setData(const ViewportData& data) {
    zData = data;
}
