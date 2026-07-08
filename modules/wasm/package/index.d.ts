export * from "./types/core-api";

import type { CoreModule, CreateCoreOptions } from "./types/core-api";

export declare const designCoreScriptUrl: string;
export declare const designCoreWasmUrl: string;

export declare function createCore(options?: CreateCoreOptions): Promise<CoreModule>;
export declare function loadDesignCoreScript(): Promise<void>;

export default createCore;
