import type { ZHandlerType, ZUIEvent } from "./ui-event/z-ui-event";

export {
  KeyCode,
  MouseButton,
  ZDrawLayerType,
  ZHandlerType,
  ZUIEventType,
} from "./ui-event/z-ui-event";
export type { ZUIEvent } from "./ui-event/z-ui-event";

export type WasmPtr = number;
export type SizeT = number;

export interface CoreWindow {
  setTitle(): void;
  dump(): void;
}

export interface CoreDocument {
  setName(): void;
}

export interface CoreCommit {
  commit(): void;
  undo(): void;
  redo(): void;
  canUndo(): boolean;
  canRedo(): boolean;
}

export interface ViewportData {
  offsetX: number;
  offsetY: number;
  scale: number;
}

export enum ZAppEventType {
  None = 0,
  DocChanged = 1,
  ViewportChanged = 2,
  HistoryChanged = 3,
  HandlerChanged = 4,
}

export interface CoreAppEvent {
  on(type: ZAppEventType, callback: (type: ZAppEventType) => void): number;
  off(type: ZAppEventType, id: number): void;
}

export interface CoreApp {
  putImage(size: SizeT, width: SizeT, height: SizeT): WasmPtr;
  draw(): void;
  randProps(): void;
  viewport(): ViewportData;
  handler(): ZHandlerType;
  onUIEvent(event: ZUIEvent): void;
  switchHandler(type: ZHandlerType): void;
  appEvent(): CoreAppEvent;
  commit(): CoreCommit;
  window(): CoreWindow;
  document(): CoreDocument;
}

export interface CreateCoreOptions {
  canvas?: HTMLCanvasElement;
  locateFile?: (path: string, scriptDirectory?: string) => string;
  noInitialRun?: boolean;
  onRuntimeInitialized?: () => void;
  print?: (text: string) => void;
  printErr?: (text: string) => void;
}

export interface CoreModule {
  HEAPU8: Uint8Array;
  getApp(): CoreApp;
}

export type CreateCoreFactory = (
  options: CreateCoreOptions,
) => Promise<CoreModule>;
