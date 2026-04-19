#pragma once

class IZEngine;

class ZWindow {
public:
    ZWindow();
    void draw();
    void setTitle();

    void testRender();

private:
    void init();

private:
    int zWidth{800};
    int zHeight{800};
    float zDpr{0};

    IZEngine* zEngine{nullptr};
};
