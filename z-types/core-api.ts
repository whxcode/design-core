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

export enum ZCommandType {
  Escape = 0,
  SwitchToCommonHandler = 1,
  DrawRectangle = 2,
  DrawEllipse = 3,
  DrawVector = 4,
  UndoDocumentHistory = 5,
  RedoDocumentHistory = 6,
  DeleteSelectedLayer = 7,
  CancelCurrentInteraction = 8,
  DownloadFile = 9,
}

export enum ZEditorModeType {
  Cursor = 0,
  DrawRectangle = 1,
  DrawOval = 2,
  DrawVector = 3,
}

export interface CoreWindow {
  setContext(context: WindowContext): void;
  dump(): void;
}

export interface CoreDocument {
  setName(): void;
}

export interface CoreCommand {
  canExecute(type: ZCommandType): boolean;
  execute(type: ZCommandType): void;
}

export interface ViewportData {
  offsetX: number;
  offsetY: number;
  scale: number;
}

export interface WindowContext {
  width: number;
  height: number;
  pixelWidth: number;
  pixelHeight: number;
  dpr: number;
}

export enum ZAppEventType {
  None = 0,
  DocChanged = 1,
  ViewportChanged = 2,
  HistoryChanged = 3,
  HandlerChanged = 4,
  HoverLayerChanged = 5,
  SelectedLayerChanged = 6,
  TraceChanged = 7,
  EditorModeChanged = 8,
}

export enum ZEditorThemeType {
  Light = 0,
  Dark = 1,
}

export interface CoreAppEvent {
  on(type: ZAppEventType, callback: (type: ZAppEventType) => void): number;
  off(type: ZAppEventType, id: number): void;
}

export interface CoreApp {
  putImage(size: SizeT, width: SizeT, height: SizeT): WasmPtr;
  draw(): void;
  viewport(): ViewportData;
  handler(): ZHandlerType;
  editorMode(): ZEditorModeType;
  onUIEvent(event: ZUIEvent): void;
  setTheme(type: ZEditorThemeType): void;
  appEvent(): CoreAppEvent;
  command(): CoreCommand;
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
