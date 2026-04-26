#pragma once

struct ViewportData {
    float offsetX{0.0f};
    float offsetY{0.0f};
    float scale{1.0f};
};

class ZViewport {
public:
    const ViewportData& data() const;
    void setData(const ViewportData& data);

private:
    ViewportData zData;
};
