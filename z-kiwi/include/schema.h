#include "kiwi.h"

namespace schema {

#ifndef INCLUDE_SCHEMA_H
#define INCLUDE_SCHEMA_H

class BinarySchema {
public:
  bool parse(kiwi::ByteBuffer &bb);
  const kiwi::BinarySchema &underlyingSchema() const { return _schema; }
  bool skipPaintField(kiwi::ByteBuffer &bb, uint32_t id) const;
  bool skipModelNodeField(kiwi::ByteBuffer &bb, uint32_t id) const;
  bool skipDocumentFileField(kiwi::ByteBuffer &bb, uint32_t id) const;

private:
  kiwi::BinarySchema _schema;
  uint32_t _indexPaint = 0;
  uint32_t _indexModelNode = 0;
  uint32_t _indexDocumentFile = 0;
};

enum class ModelType : uint32_t {
  Document = 0,
  Page = 1,
  Rectangle = 2,
  Oval = 3,
  Vector = 4,
};

class Guid;
class Size;
class Matrix;
class Point;
class PathPoint;
class PathData;
class Paint;
class ModelNode;
class DocumentFile;

class Guid {
public:
  Guid() { (void)_flags; }

  uint32_t *low();
  const uint32_t *low() const;
  void set_low(const uint32_t &value);

  uint32_t *high();
  const uint32_t *high() const;
  void set_high(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_low = {};
  uint32_t _data_high = {};
};

class Size {
public:
  Size() { (void)_flags; }

  float *width();
  const float *width() const;
  void set_width(const float &value);

  float *height();
  const float *height() const;
  void set_height(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_width = {};
  float _data_height = {};
};

class Matrix {
public:
  Matrix() { (void)_flags; }

  float *m0();
  const float *m0() const;
  void set_m0(const float &value);

  float *m1();
  const float *m1() const;
  void set_m1(const float &value);

  float *m2();
  const float *m2() const;
  void set_m2(const float &value);

  float *m3();
  const float *m3() const;
  void set_m3(const float &value);

  float *m4();
  const float *m4() const;
  void set_m4(const float &value);

  float *m5();
  const float *m5() const;
  void set_m5(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_m0 = {};
  float _data_m1 = {};
  float _data_m2 = {};
  float _data_m3 = {};
  float _data_m4 = {};
  float _data_m5 = {};
};

class Point {
public:
  Point() { (void)_flags; }

  float *x();
  const float *x() const;
  void set_x(const float &value);

  float *y();
  const float *y() const;
  void set_y(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  float _data_x = {};
  float _data_y = {};
};

class PathPoint {
public:
  PathPoint() { (void)_flags; }

  float *cornerRadius();
  const float *cornerRadius() const;
  void set_cornerRadius(const float &value);

  Point *curveFrom();
  const Point *curveFrom() const;
  void set_curveFrom(Point *value);

  Point *curveTo();
  const Point *curveTo() const;
  void set_curveTo(Point *value);

  Point *point();
  const Point *point() const;
  void set_point(Point *value);

  bool *hasCurveFrom();
  const bool *hasCurveFrom() const;
  void set_hasCurveFrom(const bool &value);

  bool *hasCurveTo();
  const bool *hasCurveTo() const;
  void set_hasCurveTo(const bool &value);

  bool *fixed();
  const bool *fixed() const;
  void set_fixed(const bool &value);

  uint32_t *curveMode();
  const uint32_t *curveMode() const;
  void set_curveMode(const uint32_t &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  Point *_data_curveFrom = {};
  Point *_data_curveTo = {};
  Point *_data_point = {};
  float _data_cornerRadius = {};
  uint32_t _data_curveMode = {};
  bool _data_hasCurveFrom = {};
  bool _data_hasCurveTo = {};
  bool _data_fixed = {};
};

class PathData {
public:
  PathData() { (void)_flags; }

  kiwi::Array<PathPoint> *points();
  const kiwi::Array<PathPoint> *points() const;
  kiwi::Array<PathPoint> &set_points(kiwi::MemoryPool &pool, uint32_t count);

  bool *isClosed();
  const bool *isClosed() const;
  void set_isClosed(const bool &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<PathPoint> _data_points = {};
  bool _data_isClosed = {};
};

class Paint {
public:
  Paint() { (void)_flags; }

  uint32_t *color();
  const uint32_t *color() const;
  void set_color(const uint32_t &value);

  float *opacity();
  const float *opacity() const;
  void set_opacity(const float &value);

  bool *visible();
  const bool *visible() const;
  void set_visible(const bool &value);

  float *strokeWidth();
  const float *strokeWidth() const;
  void set_strokeWidth(const float &value);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  uint32_t _data_color = {};
  float _data_opacity = {};
  float _data_strokeWidth = {};
  bool _data_visible = {};
};

class ModelNode {
public:
  ModelNode() { (void)_flags; }

  Guid *id();
  const Guid *id() const;
  void set_id(Guid *value);

  ModelType *type();
  const ModelType *type() const;
  void set_type(const ModelType &value);

  Guid *parentId();
  const Guid *parentId() const;
  void set_parentId(Guid *value);

  kiwi::String *name();
  const kiwi::String *name() const;
  void set_name(const kiwi::String &value);

  Size *size();
  const Size *size() const;
  void set_size(Size *value);

  Matrix *transform();
  const Matrix *transform() const;
  void set_transform(Matrix *value);

  kiwi::Array<Paint> *fills();
  const kiwi::Array<Paint> *fills() const;
  kiwi::Array<Paint> &set_fills(kiwi::MemoryPool &pool, uint32_t count);

  kiwi::Array<Paint> *strokes();
  const kiwi::Array<Paint> *strokes() const;
  kiwi::Array<Paint> &set_strokes(kiwi::MemoryPool &pool, uint32_t count);

  float *startAngle();
  const float *startAngle() const;
  void set_startAngle(const float &value);

  float *endAngle();
  const float *endAngle() const;
  void set_endAngle(const float &value);

  float *innerRadius();
  const float *innerRadius() const;
  void set_innerRadius(const float &value);

  uint32_t *windingRule();
  const uint32_t *windingRule() const;
  void set_windingRule(const uint32_t &value);

  kiwi::Array<PathData> *paths();
  const kiwi::Array<PathData> *paths() const;
  kiwi::Array<PathData> &set_paths(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  Guid *_data_id = {};
  ModelType _data_type = {};
  Guid *_data_parentId = {};
  kiwi::String _data_name = {};
  Size *_data_size = {};
  Matrix *_data_transform = {};
  kiwi::Array<Paint> _data_fills = {};
  kiwi::Array<Paint> _data_strokes = {};
  kiwi::Array<PathData> _data_paths = {};
  float _data_startAngle = {};
  float _data_endAngle = {};
  float _data_innerRadius = {};
  uint32_t _data_windingRule = {};
};

class DocumentFile {
public:
  DocumentFile() { (void)_flags; }

  uint32_t *version();
  const uint32_t *version() const;
  void set_version(const uint32_t &value);

  kiwi::Array<ModelNode> *children();
  const kiwi::Array<ModelNode> *children() const;
  kiwi::Array<ModelNode> &set_children(kiwi::MemoryPool &pool, uint32_t count);

  bool encode(kiwi::ByteBuffer &bb);
  bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
  uint32_t _flags[1] = {};
  kiwi::Array<ModelNode> _data_children = {};
  uint32_t _data_version = {};
};

#endif
#ifdef IMPLEMENT_SCHEMA_H

bool BinarySchema::parse(kiwi::ByteBuffer &bb) {
  if (!_schema.parse(bb)) return false;
  _schema.findDefinition("Paint", _indexPaint);
  _schema.findDefinition("ModelNode", _indexModelNode);
  _schema.findDefinition("DocumentFile", _indexDocumentFile);
  return true;
}

bool BinarySchema::skipPaintField(kiwi::ByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexPaint, id);
}

bool BinarySchema::skipModelNodeField(kiwi::ByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexModelNode, id);
}

bool BinarySchema::skipDocumentFileField(kiwi::ByteBuffer &bb, uint32_t id) const {
  return _schema.skipField(bb, _indexDocumentFile, id);
}

uint32_t *Guid::low() {
  return _flags[0] & 1 ? &_data_low : nullptr;
}

const uint32_t *Guid::low() const {
  return _flags[0] & 1 ? &_data_low : nullptr;
}

void Guid::set_low(const uint32_t &value) {
  _flags[0] |= 1; _data_low = value;
}

uint32_t *Guid::high() {
  return _flags[0] & 2 ? &_data_high : nullptr;
}

const uint32_t *Guid::high() const {
  return _flags[0] & 2 ? &_data_high : nullptr;
}

void Guid::set_high(const uint32_t &value) {
  _flags[0] |= 2; _data_high = value;
}

bool Guid::encode(kiwi::ByteBuffer &_bb) {
  if (low() == nullptr) return false;
  _bb.writeVarUint(_data_low);
  if (high() == nullptr) return false;
  _bb.writeVarUint(_data_high);
  return true;
}

bool Guid::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarUint(_data_low)) return false;
  set_low(_data_low);
  if (!_bb.readVarUint(_data_high)) return false;
  set_high(_data_high);
  return true;
}

float *Size::width() {
  return _flags[0] & 1 ? &_data_width : nullptr;
}

const float *Size::width() const {
  return _flags[0] & 1 ? &_data_width : nullptr;
}

void Size::set_width(const float &value) {
  _flags[0] |= 1; _data_width = value;
}

float *Size::height() {
  return _flags[0] & 2 ? &_data_height : nullptr;
}

const float *Size::height() const {
  return _flags[0] & 2 ? &_data_height : nullptr;
}

void Size::set_height(const float &value) {
  _flags[0] |= 2; _data_height = value;
}

bool Size::encode(kiwi::ByteBuffer &_bb) {
  if (width() == nullptr) return false;
  _bb.writeVarFloat(_data_width);
  if (height() == nullptr) return false;
  _bb.writeVarFloat(_data_height);
  return true;
}

bool Size::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarFloat(_data_width)) return false;
  set_width(_data_width);
  if (!_bb.readVarFloat(_data_height)) return false;
  set_height(_data_height);
  return true;
}

float *Matrix::m0() {
  return _flags[0] & 1 ? &_data_m0 : nullptr;
}

const float *Matrix::m0() const {
  return _flags[0] & 1 ? &_data_m0 : nullptr;
}

void Matrix::set_m0(const float &value) {
  _flags[0] |= 1; _data_m0 = value;
}

float *Matrix::m1() {
  return _flags[0] & 2 ? &_data_m1 : nullptr;
}

const float *Matrix::m1() const {
  return _flags[0] & 2 ? &_data_m1 : nullptr;
}

void Matrix::set_m1(const float &value) {
  _flags[0] |= 2; _data_m1 = value;
}

float *Matrix::m2() {
  return _flags[0] & 4 ? &_data_m2 : nullptr;
}

const float *Matrix::m2() const {
  return _flags[0] & 4 ? &_data_m2 : nullptr;
}

void Matrix::set_m2(const float &value) {
  _flags[0] |= 4; _data_m2 = value;
}

float *Matrix::m3() {
  return _flags[0] & 8 ? &_data_m3 : nullptr;
}

const float *Matrix::m3() const {
  return _flags[0] & 8 ? &_data_m3 : nullptr;
}

void Matrix::set_m3(const float &value) {
  _flags[0] |= 8; _data_m3 = value;
}

float *Matrix::m4() {
  return _flags[0] & 16 ? &_data_m4 : nullptr;
}

const float *Matrix::m4() const {
  return _flags[0] & 16 ? &_data_m4 : nullptr;
}

void Matrix::set_m4(const float &value) {
  _flags[0] |= 16; _data_m4 = value;
}

float *Matrix::m5() {
  return _flags[0] & 32 ? &_data_m5 : nullptr;
}

const float *Matrix::m5() const {
  return _flags[0] & 32 ? &_data_m5 : nullptr;
}

void Matrix::set_m5(const float &value) {
  _flags[0] |= 32; _data_m5 = value;
}

bool Matrix::encode(kiwi::ByteBuffer &_bb) {
  if (m0() == nullptr) return false;
  _bb.writeVarFloat(_data_m0);
  if (m1() == nullptr) return false;
  _bb.writeVarFloat(_data_m1);
  if (m2() == nullptr) return false;
  _bb.writeVarFloat(_data_m2);
  if (m3() == nullptr) return false;
  _bb.writeVarFloat(_data_m3);
  if (m4() == nullptr) return false;
  _bb.writeVarFloat(_data_m4);
  if (m5() == nullptr) return false;
  _bb.writeVarFloat(_data_m5);
  return true;
}

bool Matrix::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarFloat(_data_m0)) return false;
  set_m0(_data_m0);
  if (!_bb.readVarFloat(_data_m1)) return false;
  set_m1(_data_m1);
  if (!_bb.readVarFloat(_data_m2)) return false;
  set_m2(_data_m2);
  if (!_bb.readVarFloat(_data_m3)) return false;
  set_m3(_data_m3);
  if (!_bb.readVarFloat(_data_m4)) return false;
  set_m4(_data_m4);
  if (!_bb.readVarFloat(_data_m5)) return false;
  set_m5(_data_m5);
  return true;
}

float *Point::x() {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

const float *Point::x() const {
  return _flags[0] & 1 ? &_data_x : nullptr;
}

void Point::set_x(const float &value) {
  _flags[0] |= 1; _data_x = value;
}

float *Point::y() {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

const float *Point::y() const {
  return _flags[0] & 2 ? &_data_y : nullptr;
}

void Point::set_y(const float &value) {
  _flags[0] |= 2; _data_y = value;
}

bool Point::encode(kiwi::ByteBuffer &_bb) {
  if (x() == nullptr) return false;
  _bb.writeVarFloat(_data_x);
  if (y() == nullptr) return false;
  _bb.writeVarFloat(_data_y);
  return true;
}

bool Point::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarFloat(_data_x)) return false;
  set_x(_data_x);
  if (!_bb.readVarFloat(_data_y)) return false;
  set_y(_data_y);
  return true;
}

float *PathPoint::cornerRadius() {
  return _flags[0] & 1 ? &_data_cornerRadius : nullptr;
}

const float *PathPoint::cornerRadius() const {
  return _flags[0] & 1 ? &_data_cornerRadius : nullptr;
}

void PathPoint::set_cornerRadius(const float &value) {
  _flags[0] |= 1; _data_cornerRadius = value;
}

Point *PathPoint::curveFrom() {
  return _data_curveFrom;
}

const Point *PathPoint::curveFrom() const {
  return _data_curveFrom;
}

void PathPoint::set_curveFrom(Point *value) {
  _data_curveFrom = value;
}

Point *PathPoint::curveTo() {
  return _data_curveTo;
}

const Point *PathPoint::curveTo() const {
  return _data_curveTo;
}

void PathPoint::set_curveTo(Point *value) {
  _data_curveTo = value;
}

Point *PathPoint::point() {
  return _data_point;
}

const Point *PathPoint::point() const {
  return _data_point;
}

void PathPoint::set_point(Point *value) {
  _data_point = value;
}

bool *PathPoint::hasCurveFrom() {
  return _flags[0] & 16 ? &_data_hasCurveFrom : nullptr;
}

const bool *PathPoint::hasCurveFrom() const {
  return _flags[0] & 16 ? &_data_hasCurveFrom : nullptr;
}

void PathPoint::set_hasCurveFrom(const bool &value) {
  _flags[0] |= 16; _data_hasCurveFrom = value;
}

bool *PathPoint::hasCurveTo() {
  return _flags[0] & 32 ? &_data_hasCurveTo : nullptr;
}

const bool *PathPoint::hasCurveTo() const {
  return _flags[0] & 32 ? &_data_hasCurveTo : nullptr;
}

void PathPoint::set_hasCurveTo(const bool &value) {
  _flags[0] |= 32; _data_hasCurveTo = value;
}

bool *PathPoint::fixed() {
  return _flags[0] & 64 ? &_data_fixed : nullptr;
}

const bool *PathPoint::fixed() const {
  return _flags[0] & 64 ? &_data_fixed : nullptr;
}

void PathPoint::set_fixed(const bool &value) {
  _flags[0] |= 64; _data_fixed = value;
}

uint32_t *PathPoint::curveMode() {
  return _flags[0] & 128 ? &_data_curveMode : nullptr;
}

const uint32_t *PathPoint::curveMode() const {
  return _flags[0] & 128 ? &_data_curveMode : nullptr;
}

void PathPoint::set_curveMode(const uint32_t &value) {
  _flags[0] |= 128; _data_curveMode = value;
}

bool PathPoint::encode(kiwi::ByteBuffer &_bb) {
  if (cornerRadius() == nullptr) return false;
  _bb.writeVarFloat(_data_cornerRadius);
  if (curveFrom() == nullptr) return false;
  if (!_data_curveFrom->encode(_bb)) return false;
  if (curveTo() == nullptr) return false;
  if (!_data_curveTo->encode(_bb)) return false;
  if (point() == nullptr) return false;
  if (!_data_point->encode(_bb)) return false;
  if (hasCurveFrom() == nullptr) return false;
  _bb.writeByte(_data_hasCurveFrom);
  if (hasCurveTo() == nullptr) return false;
  _bb.writeByte(_data_hasCurveTo);
  if (fixed() == nullptr) return false;
  _bb.writeByte(_data_fixed);
  if (curveMode() == nullptr) return false;
  _bb.writeVarUint(_data_curveMode);
  return true;
}

bool PathPoint::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  if (!_bb.readVarFloat(_data_cornerRadius)) return false;
  set_cornerRadius(_data_cornerRadius);
  _data_curveFrom = _pool.allocate<Point>();
  if (!_data_curveFrom->decode(_bb, _pool, _schema)) return false;
  _data_curveTo = _pool.allocate<Point>();
  if (!_data_curveTo->decode(_bb, _pool, _schema)) return false;
  _data_point = _pool.allocate<Point>();
  if (!_data_point->decode(_bb, _pool, _schema)) return false;
  if (!_bb.readByte(_data_hasCurveFrom)) return false;
  set_hasCurveFrom(_data_hasCurveFrom);
  if (!_bb.readByte(_data_hasCurveTo)) return false;
  set_hasCurveTo(_data_hasCurveTo);
  if (!_bb.readByte(_data_fixed)) return false;
  set_fixed(_data_fixed);
  if (!_bb.readVarUint(_data_curveMode)) return false;
  set_curveMode(_data_curveMode);
  return true;
}

kiwi::Array<PathPoint> *PathData::points() {
  return _flags[0] & 1 ? &_data_points : nullptr;
}

const kiwi::Array<PathPoint> *PathData::points() const {
  return _flags[0] & 1 ? &_data_points : nullptr;
}

kiwi::Array<PathPoint> &PathData::set_points(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 1; return _data_points = pool.array<PathPoint>(count);
}

bool *PathData::isClosed() {
  return _flags[0] & 2 ? &_data_isClosed : nullptr;
}

const bool *PathData::isClosed() const {
  return _flags[0] & 2 ? &_data_isClosed : nullptr;
}

void PathData::set_isClosed(const bool &value) {
  _flags[0] |= 2; _data_isClosed = value;
}

bool PathData::encode(kiwi::ByteBuffer &_bb) {
  if (points() == nullptr) return false;
  _bb.writeVarUint(_data_points.size());
  for (PathPoint &_it : _data_points) if (!_it.encode(_bb)) return false;
  if (isClosed() == nullptr) return false;
  _bb.writeByte(_data_isClosed);
  return true;
}

bool PathData::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  if (!_bb.readVarUint(_count)) return false;
  for (PathPoint &_it : set_points(_pool, _count)) if (!_it.decode(_bb, _pool, _schema)) return false;
  if (!_bb.readByte(_data_isClosed)) return false;
  set_isClosed(_data_isClosed);
  return true;
}

uint32_t *Paint::color() {
  return _flags[0] & 1 ? &_data_color : nullptr;
}

const uint32_t *Paint::color() const {
  return _flags[0] & 1 ? &_data_color : nullptr;
}

void Paint::set_color(const uint32_t &value) {
  _flags[0] |= 1; _data_color = value;
}

float *Paint::opacity() {
  return _flags[0] & 2 ? &_data_opacity : nullptr;
}

const float *Paint::opacity() const {
  return _flags[0] & 2 ? &_data_opacity : nullptr;
}

void Paint::set_opacity(const float &value) {
  _flags[0] |= 2; _data_opacity = value;
}

bool *Paint::visible() {
  return _flags[0] & 4 ? &_data_visible : nullptr;
}

const bool *Paint::visible() const {
  return _flags[0] & 4 ? &_data_visible : nullptr;
}

void Paint::set_visible(const bool &value) {
  _flags[0] |= 4; _data_visible = value;
}

float *Paint::strokeWidth() {
  return _flags[0] & 8 ? &_data_strokeWidth : nullptr;
}

const float *Paint::strokeWidth() const {
  return _flags[0] & 8 ? &_data_strokeWidth : nullptr;
}

void Paint::set_strokeWidth(const float &value) {
  _flags[0] |= 8; _data_strokeWidth = value;
}

bool Paint::encode(kiwi::ByteBuffer &_bb) {
  if (color() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_color);
  }
  if (opacity() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarFloat(_data_opacity);
  }
  if (visible() != nullptr) {
    _bb.writeVarUint(3);
    _bb.writeByte(_data_visible);
  }
  if (strokeWidth() != nullptr) {
    _bb.writeVarUint(4);
    _bb.writeVarFloat(_data_strokeWidth);
  }
  _bb.writeVarUint(0);
  return true;
}

bool Paint::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_color)) return false;
        set_color(_data_color);
        break;
      }
      case 2: {
        if (!_bb.readVarFloat(_data_opacity)) return false;
        set_opacity(_data_opacity);
        break;
      }
      case 3: {
        if (!_bb.readByte(_data_visible)) return false;
        set_visible(_data_visible);
        break;
      }
      case 4: {
        if (!_bb.readVarFloat(_data_strokeWidth)) return false;
        set_strokeWidth(_data_strokeWidth);
        break;
      }
      default: {
        if (!_schema || !_schema->skipPaintField(_bb, _type)) return false;
        break;
      }
    }
  }
}

Guid *ModelNode::id() {
  return _data_id;
}

const Guid *ModelNode::id() const {
  return _data_id;
}

void ModelNode::set_id(Guid *value) {
  _data_id = value;
}

ModelType *ModelNode::type() {
  return _flags[0] & 2 ? &_data_type : nullptr;
}

const ModelType *ModelNode::type() const {
  return _flags[0] & 2 ? &_data_type : nullptr;
}

void ModelNode::set_type(const ModelType &value) {
  _flags[0] |= 2; _data_type = value;
}

Guid *ModelNode::parentId() {
  return _data_parentId;
}

const Guid *ModelNode::parentId() const {
  return _data_parentId;
}

void ModelNode::set_parentId(Guid *value) {
  _data_parentId = value;
}

kiwi::String *ModelNode::name() {
  return _flags[0] & 8 ? &_data_name : nullptr;
}

const kiwi::String *ModelNode::name() const {
  return _flags[0] & 8 ? &_data_name : nullptr;
}

void ModelNode::set_name(const kiwi::String &value) {
  _flags[0] |= 8; _data_name = value;
}

Size *ModelNode::size() {
  return _data_size;
}

const Size *ModelNode::size() const {
  return _data_size;
}

void ModelNode::set_size(Size *value) {
  _data_size = value;
}

Matrix *ModelNode::transform() {
  return _data_transform;
}

const Matrix *ModelNode::transform() const {
  return _data_transform;
}

void ModelNode::set_transform(Matrix *value) {
  _data_transform = value;
}

kiwi::Array<Paint> *ModelNode::fills() {
  return _flags[0] & 64 ? &_data_fills : nullptr;
}

const kiwi::Array<Paint> *ModelNode::fills() const {
  return _flags[0] & 64 ? &_data_fills : nullptr;
}

kiwi::Array<Paint> &ModelNode::set_fills(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 64; return _data_fills = pool.array<Paint>(count);
}

kiwi::Array<Paint> *ModelNode::strokes() {
  return _flags[0] & 128 ? &_data_strokes : nullptr;
}

const kiwi::Array<Paint> *ModelNode::strokes() const {
  return _flags[0] & 128 ? &_data_strokes : nullptr;
}

kiwi::Array<Paint> &ModelNode::set_strokes(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 128; return _data_strokes = pool.array<Paint>(count);
}

float *ModelNode::startAngle() {
  return _flags[0] & 256 ? &_data_startAngle : nullptr;
}

const float *ModelNode::startAngle() const {
  return _flags[0] & 256 ? &_data_startAngle : nullptr;
}

void ModelNode::set_startAngle(const float &value) {
  _flags[0] |= 256; _data_startAngle = value;
}

float *ModelNode::endAngle() {
  return _flags[0] & 512 ? &_data_endAngle : nullptr;
}

const float *ModelNode::endAngle() const {
  return _flags[0] & 512 ? &_data_endAngle : nullptr;
}

void ModelNode::set_endAngle(const float &value) {
  _flags[0] |= 512; _data_endAngle = value;
}

float *ModelNode::innerRadius() {
  return _flags[0] & 1024 ? &_data_innerRadius : nullptr;
}

const float *ModelNode::innerRadius() const {
  return _flags[0] & 1024 ? &_data_innerRadius : nullptr;
}

void ModelNode::set_innerRadius(const float &value) {
  _flags[0] |= 1024; _data_innerRadius = value;
}

uint32_t *ModelNode::windingRule() {
  return _flags[0] & 2048 ? &_data_windingRule : nullptr;
}

const uint32_t *ModelNode::windingRule() const {
  return _flags[0] & 2048 ? &_data_windingRule : nullptr;
}

void ModelNode::set_windingRule(const uint32_t &value) {
  _flags[0] |= 2048; _data_windingRule = value;
}

kiwi::Array<PathData> *ModelNode::paths() {
  return _flags[0] & 4096 ? &_data_paths : nullptr;
}

const kiwi::Array<PathData> *ModelNode::paths() const {
  return _flags[0] & 4096 ? &_data_paths : nullptr;
}

kiwi::Array<PathData> &ModelNode::set_paths(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 4096; return _data_paths = pool.array<PathData>(count);
}

bool ModelNode::encode(kiwi::ByteBuffer &_bb) {
  if (id() != nullptr) {
    _bb.writeVarUint(1);
    if (!_data_id->encode(_bb)) return false;
  }
  if (type() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarUint(static_cast<uint32_t>(_data_type));
  }
  if (parentId() != nullptr) {
    _bb.writeVarUint(3);
    if (!_data_parentId->encode(_bb)) return false;
  }
  if (name() != nullptr) {
    _bb.writeVarUint(4);
    _bb.writeString(_data_name.c_str());
  }
  if (size() != nullptr) {
    _bb.writeVarUint(5);
    if (!_data_size->encode(_bb)) return false;
  }
  if (transform() != nullptr) {
    _bb.writeVarUint(6);
    if (!_data_transform->encode(_bb)) return false;
  }
  if (fills() != nullptr) {
    _bb.writeVarUint(7);
    _bb.writeVarUint(_data_fills.size());
    for (Paint &_it : _data_fills) if (!_it.encode(_bb)) return false;
  }
  if (strokes() != nullptr) {
    _bb.writeVarUint(8);
    _bb.writeVarUint(_data_strokes.size());
    for (Paint &_it : _data_strokes) if (!_it.encode(_bb)) return false;
  }
  if (startAngle() != nullptr) {
    _bb.writeVarUint(9);
    _bb.writeVarFloat(_data_startAngle);
  }
  if (endAngle() != nullptr) {
    _bb.writeVarUint(10);
    _bb.writeVarFloat(_data_endAngle);
  }
  if (innerRadius() != nullptr) {
    _bb.writeVarUint(11);
    _bb.writeVarFloat(_data_innerRadius);
  }
  if (windingRule() != nullptr) {
    _bb.writeVarUint(12);
    _bb.writeVarUint(_data_windingRule);
  }
  if (paths() != nullptr) {
    _bb.writeVarUint(13);
    _bb.writeVarUint(_data_paths.size());
    for (PathData &_it : _data_paths) if (!_it.encode(_bb)) return false;
  }
  _bb.writeVarUint(0);
  return true;
}

bool ModelNode::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        _data_id = _pool.allocate<Guid>();
        if (!_data_id->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 2: {
        if (!_bb.readVarUint(reinterpret_cast<uint32_t &>(_data_type))) return false;
        set_type(_data_type);
        break;
      }
      case 3: {
        _data_parentId = _pool.allocate<Guid>();
        if (!_data_parentId->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 4: {
        if (!_bb.readString(_data_name, _pool)) return false;
        set_name(_data_name);
        break;
      }
      case 5: {
        _data_size = _pool.allocate<Size>();
        if (!_data_size->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 6: {
        _data_transform = _pool.allocate<Matrix>();
        if (!_data_transform->decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 7: {
        if (!_bb.readVarUint(_count)) return false;
        for (Paint &_it : set_fills(_pool, _count)) if (!_it.decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 8: {
        if (!_bb.readVarUint(_count)) return false;
        for (Paint &_it : set_strokes(_pool, _count)) if (!_it.decode(_bb, _pool, _schema)) return false;
        break;
      }
      case 9: {
        if (!_bb.readVarFloat(_data_startAngle)) return false;
        set_startAngle(_data_startAngle);
        break;
      }
      case 10: {
        if (!_bb.readVarFloat(_data_endAngle)) return false;
        set_endAngle(_data_endAngle);
        break;
      }
      case 11: {
        if (!_bb.readVarFloat(_data_innerRadius)) return false;
        set_innerRadius(_data_innerRadius);
        break;
      }
      case 12: {
        if (!_bb.readVarUint(_data_windingRule)) return false;
        set_windingRule(_data_windingRule);
        break;
      }
      case 13: {
        if (!_bb.readVarUint(_count)) return false;
        for (PathData &_it : set_paths(_pool, _count)) if (!_it.decode(_bb, _pool, _schema)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipModelNodeField(_bb, _type)) return false;
        break;
      }
    }
  }
}

uint32_t *DocumentFile::version() {
  return _flags[0] & 1 ? &_data_version : nullptr;
}

const uint32_t *DocumentFile::version() const {
  return _flags[0] & 1 ? &_data_version : nullptr;
}

void DocumentFile::set_version(const uint32_t &value) {
  _flags[0] |= 1; _data_version = value;
}

kiwi::Array<ModelNode> *DocumentFile::children() {
  return _flags[0] & 2 ? &_data_children : nullptr;
}

const kiwi::Array<ModelNode> *DocumentFile::children() const {
  return _flags[0] & 2 ? &_data_children : nullptr;
}

kiwi::Array<ModelNode> &DocumentFile::set_children(kiwi::MemoryPool &pool, uint32_t count) {
  _flags[0] |= 2; return _data_children = pool.array<ModelNode>(count);
}

bool DocumentFile::encode(kiwi::ByteBuffer &_bb) {
  if (version() != nullptr) {
    _bb.writeVarUint(1);
    _bb.writeVarUint(_data_version);
  }
  if (children() != nullptr) {
    _bb.writeVarUint(2);
    _bb.writeVarUint(_data_children.size());
    for (ModelNode &_it : _data_children) if (!_it.encode(_bb)) return false;
  }
  _bb.writeVarUint(0);
  return true;
}

bool DocumentFile::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema) {
  uint32_t _count;
  while (true) {
    uint32_t _type;
    if (!_bb.readVarUint(_type)) return false;
    switch (_type) {
      case 0:
        return true;
      case 1: {
        if (!_bb.readVarUint(_data_version)) return false;
        set_version(_data_version);
        break;
      }
      case 2: {
        if (!_bb.readVarUint(_count)) return false;
        for (ModelNode &_it : set_children(_pool, _count)) if (!_it.decode(_bb, _pool, _schema)) return false;
        break;
      }
      default: {
        if (!_schema || !_schema->skipDocumentFileField(_bb, _type)) return false;
        break;
      }
    }
  }
}

#endif

}
