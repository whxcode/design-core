export type WasmPtr = number;

export interface CoreWindow {
  setTitle(): void;
  dump(): void;
}

export interface CoreDocument {
  setName(): void;
}

export interface CoreApp {
  calloc(size: number): WasmPtr;
  free(ptr: WasmPtr): void;
  putImage1(buffer: Uint8Array): void;
  putImage2(ptr: WasmPtr, len: number): void;
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

export type CreateCoreFactory = (options: CreateCoreOptions) => Promise<CoreModule>;
