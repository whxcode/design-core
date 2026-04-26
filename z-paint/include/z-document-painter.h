#pragma once

#include "z-paint/include/z-painter.h"
#include "z-tools/include/z-type.h"

class ZPage;

class ZDocumentPainter : public ZPainter {
public:
    void setPage(const z_sp<ZPage>& page);
    void draw(IZEngine* engine) override;

private:
    z_sp<ZPage> zPage{nullptr};
};
