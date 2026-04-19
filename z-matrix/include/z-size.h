#pragma once

class ZSize {
public:
    ZSize();
    ZSize(float w, float h);

    static ZSize Make(float w, float h);
    static ZSize MakeEmpty();

    void set(float w, float h);
    bool isEmpty() const;

    float width() const;
    float height() const;

    bool operator==(const ZSize& other) const;
    bool operator!=(const ZSize& other) const;

private:
    float zWidth;
    float zHeight;
};
