export namespace schema {
  export type ModelType =
    "Document" |
    "Page" |
    "Rectangle" |
    "Oval" |
    "Vector";

  export interface Guid {
    low: number;
    high: number;
  }

  export interface Size {
    width: number;
    height: number;
  }

  export interface Matrix {
    m0: number;
    m1: number;
    m2: number;
    m3: number;
    m4: number;
    m5: number;
  }

  export interface Point {
    x: number;
    y: number;
  }

  export interface PathPoint {
    cornerRadius: number;
    curveFrom: Point;
    curveTo: Point;
    point: Point;
    hasCurveFrom: boolean;
    hasCurveTo: boolean;
    fixed: boolean;
    curveMode: number;
  }

  export interface PathData {
    points: PathPoint[];
    isClosed: boolean;
  }

  export interface Paint {
    color?: number;
    opacity?: number;
    visible?: boolean;
    strokeWidth?: number;
  }

  export interface ModelNode {
    id?: Guid;
    type?: ModelType;
    parentId?: Guid;
    name?: string;
    size?: Size;
    transform?: Matrix;
    fills?: Paint[];
    strokes?: Paint[];
    startAngle?: number;
    endAngle?: number;
    innerRadius?: number;
    windingRule?: number;
    paths?: PathData[];
  }

  export interface DocumentFile {
    version?: number;
    children?: ModelNode[];
  }

  export interface Schema {
    encodeGuid(message: Guid): Uint8Array;
    decodeGuid(buffer: Uint8Array): Guid;
    encodeSize(message: Size): Uint8Array;
    decodeSize(buffer: Uint8Array): Size;
    encodeMatrix(message: Matrix): Uint8Array;
    decodeMatrix(buffer: Uint8Array): Matrix;
    encodePoint(message: Point): Uint8Array;
    decodePoint(buffer: Uint8Array): Point;
    ModelType: any;
    encodePathPoint(message: PathPoint): Uint8Array;
    decodePathPoint(buffer: Uint8Array): PathPoint;
    encodePathData(message: PathData): Uint8Array;
    decodePathData(buffer: Uint8Array): PathData;
    encodePaint(message: Paint): Uint8Array;
    decodePaint(buffer: Uint8Array): Paint;
    encodeModelNode(message: ModelNode): Uint8Array;
    decodeModelNode(buffer: Uint8Array): ModelNode;
    encodeDocumentFile(message: DocumentFile): Uint8Array;
    decodeDocumentFile(buffer: Uint8Array): DocumentFile;
  }
}
