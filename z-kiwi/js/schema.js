import { ByteBuffer } from "kiwi-schema";
var schema = { ByteBuffer };

schema["decodeGuid"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["low"] = bb.readVarUint();
  result["high"] = bb.readVarUint();
  return result;
};

schema["encodeGuid"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["low"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"low\"");
  }

  var value = message["high"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"high\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodeSize"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["width"] = bb.readVarFloat();
  result["height"] = bb.readVarFloat();
  return result;
};

schema["encodeSize"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["width"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"width\"");
  }

  var value = message["height"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"height\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodeMatrix"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["m0"] = bb.readVarFloat();
  result["m1"] = bb.readVarFloat();
  result["m2"] = bb.readVarFloat();
  result["m3"] = bb.readVarFloat();
  result["m4"] = bb.readVarFloat();
  result["m5"] = bb.readVarFloat();
  return result;
};

schema["encodeMatrix"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["m0"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m0\"");
  }

  var value = message["m1"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m1\"");
  }

  var value = message["m2"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m2\"");
  }

  var value = message["m3"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m3\"");
  }

  var value = message["m4"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m4\"");
  }

  var value = message["m5"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"m5\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodePoint"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["x"] = bb.readVarFloat();
  result["y"] = bb.readVarFloat();
  return result;
};

schema["encodePoint"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["x"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"x\"");
  }

  var value = message["y"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"y\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};
schema["ModelType"] = {
  "0": "Document",
  "1": "Page",
  "2": "Rectangle",
  "3": "Oval",
  "4": "Vector",
  "Document": 0,
  "Page": 1,
  "Rectangle": 2,
  "Oval": 3,
  "Vector": 4
};

schema["decodePathPoint"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  result["cornerRadius"] = bb.readVarFloat();
  result["curveFrom"] = this["decodePoint"](bb);
  result["curveTo"] = this["decodePoint"](bb);
  result["point"] = this["decodePoint"](bb);
  result["hasCurveFrom"] = !!bb.readByte();
  result["hasCurveTo"] = !!bb.readByte();
  result["fixed"] = !!bb.readByte();
  result["curveMode"] = bb.readVarUint();
  return result;
};

schema["encodePathPoint"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["cornerRadius"];
  if (value != null) {
    bb.writeVarFloat(value);
  } else {
    throw new Error("Missing required field \"cornerRadius\"");
  }

  var value = message["curveFrom"];
  if (value != null) {
    this["encodePoint"](value, bb);
  } else {
    throw new Error("Missing required field \"curveFrom\"");
  }

  var value = message["curveTo"];
  if (value != null) {
    this["encodePoint"](value, bb);
  } else {
    throw new Error("Missing required field \"curveTo\"");
  }

  var value = message["point"];
  if (value != null) {
    this["encodePoint"](value, bb);
  } else {
    throw new Error("Missing required field \"point\"");
  }

  var value = message["hasCurveFrom"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"hasCurveFrom\"");
  }

  var value = message["hasCurveTo"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"hasCurveTo\"");
  }

  var value = message["fixed"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"fixed\"");
  }

  var value = message["curveMode"];
  if (value != null) {
    bb.writeVarUint(value);
  } else {
    throw new Error("Missing required field \"curveMode\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodePathData"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  var length = bb.readVarUint();
  var values = result["points"] = Array(length);
  for (var i = 0; i < length; i++) values[i] = this["decodePathPoint"](bb);
  result["isClosed"] = !!bb.readByte();
  return result;
};

schema["encodePathData"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["points"];
  if (value != null) {
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      this["encodePathPoint"](value, bb);
    }
  } else {
    throw new Error("Missing required field \"points\"");
  }

  var value = message["isClosed"];
  if (value != null) {
    bb.writeByte(value);
  } else {
    throw new Error("Missing required field \"isClosed\"");
  }

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodePaint"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
      case 0:
        return result;

      case 1:
        result["color"] = bb.readVarUint();
        break;

      case 2:
        result["opacity"] = bb.readVarFloat();
        break;

      case 3:
        result["visible"] = !!bb.readByte();
        break;

      case 4:
        result["strokeWidth"] = bb.readVarFloat();
        break;

      default:
        throw new Error("Attempted to parse invalid message");
    }
  }
};

schema["encodePaint"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["color"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["opacity"];
  if (value != null) {
    bb.writeVarUint(2);
    bb.writeVarFloat(value);
  }

  var value = message["visible"];
  if (value != null) {
    bb.writeVarUint(3);
    bb.writeByte(value);
  }

  var value = message["strokeWidth"];
  if (value != null) {
    bb.writeVarUint(4);
    bb.writeVarFloat(value);
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodeModelNode"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
      case 0:
        return result;

      case 1:
        result["id"] = this["decodeGuid"](bb);
        break;

      case 2:
        result["type"] = this["ModelType"][bb.readVarUint()];
        break;

      case 3:
        result["parentId"] = this["decodeGuid"](bb);
        break;

      case 4:
        result["name"] = bb.readString();
        break;

      case 5:
        result["size"] = this["decodeSize"](bb);
        break;

      case 6:
        result["transform"] = this["decodeMatrix"](bb);
        break;

      case 7:
        var length = bb.readVarUint();
        var values = result["fills"] = Array(length);
        for (var i = 0; i < length; i++) values[i] = this["decodePaint"](bb);
        break;

      case 8:
        var length = bb.readVarUint();
        var values = result["strokes"] = Array(length);
        for (var i = 0; i < length; i++) values[i] = this["decodePaint"](bb);
        break;

      case 9:
        result["startAngle"] = bb.readVarFloat();
        break;

      case 10:
        result["endAngle"] = bb.readVarFloat();
        break;

      case 11:
        result["innerRadius"] = bb.readVarFloat();
        break;

      case 12:
        result["windingRule"] = bb.readVarUint();
        break;

      case 13:
        var length = bb.readVarUint();
        var values = result["paths"] = Array(length);
        for (var i = 0; i < length; i++) values[i] = this["decodePathData"](bb);
        break;

      default:
        throw new Error("Attempted to parse invalid message");
    }
  }
};

schema["encodeModelNode"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["id"];
  if (value != null) {
    bb.writeVarUint(1);
    this["encodeGuid"](value, bb);
  }

  var value = message["type"];
  if (value != null) {
    bb.writeVarUint(2);
    var encoded = this["ModelType"][value]; if (encoded === void 0) throw new Error("Invalid value " + JSON.stringify(value) + " for enum \"ModelType\""); bb.writeVarUint(encoded);
  }

  var value = message["parentId"];
  if (value != null) {
    bb.writeVarUint(3);
    this["encodeGuid"](value, bb);
  }

  var value = message["name"];
  if (value != null) {
    bb.writeVarUint(4);
    bb.writeString(value);
  }

  var value = message["size"];
  if (value != null) {
    bb.writeVarUint(5);
    this["encodeSize"](value, bb);
  }

  var value = message["transform"];
  if (value != null) {
    bb.writeVarUint(6);
    this["encodeMatrix"](value, bb);
  }

  var value = message["fills"];
  if (value != null) {
    bb.writeVarUint(7);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      this["encodePaint"](value, bb);
    }
  }

  var value = message["strokes"];
  if (value != null) {
    bb.writeVarUint(8);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      this["encodePaint"](value, bb);
    }
  }

  var value = message["startAngle"];
  if (value != null) {
    bb.writeVarUint(9);
    bb.writeVarFloat(value);
  }

  var value = message["endAngle"];
  if (value != null) {
    bb.writeVarUint(10);
    bb.writeVarFloat(value);
  }

  var value = message["innerRadius"];
  if (value != null) {
    bb.writeVarUint(11);
    bb.writeVarFloat(value);
  }

  var value = message["windingRule"];
  if (value != null) {
    bb.writeVarUint(12);
    bb.writeVarUint(value);
  }

  var value = message["paths"];
  if (value != null) {
    bb.writeVarUint(13);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      this["encodePathData"](value, bb);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

schema["decodeDocumentFile"] = function (bb) {
  var result = {};
  if (!(bb instanceof this.ByteBuffer)) {
    bb = new this.ByteBuffer(bb);
  }

  while (true) {
    switch (bb.readVarUint()) {
      case 0:
        return result;

      case 1:
        result["version"] = bb.readVarUint();
        break;

      case 2:
        var length = bb.readVarUint();
        var values = result["children"] = Array(length);
        for (var i = 0; i < length; i++) values[i] = this["decodeModelNode"](bb);
        break;

      default:
        throw new Error("Attempted to parse invalid message");
    }
  }
};

schema["encodeDocumentFile"] = function (message, bb) {
  var isTopLevel = !bb;
  if (isTopLevel) bb = new this.ByteBuffer();

  var value = message["version"];
  if (value != null) {
    bb.writeVarUint(1);
    bb.writeVarUint(value);
  }

  var value = message["children"];
  if (value != null) {
    bb.writeVarUint(2);
    var values = value, n = values.length;
    bb.writeVarUint(n);
    for (var i = 0; i < n; i++) {
      value = values[i];
      this["encodeModelNode"](value, bb);
    }
  }
  bb.writeVarUint(0);

  if (isTopLevel) return bb.toUint8Array();
};

export { schema };
