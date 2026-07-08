import wasmUrl from "./DesignCore.wasm?url";
import { designCoreScriptUrl, loadDesignCoreScript } from "./index.mjs";

export { designCoreScriptUrl, loadDesignCoreScript };
export const designCoreWasmUrl = wasmUrl;

export async function createCore(options = {}) {
  await loadDesignCoreScript();

  const createCoreFactory = globalThis.createCore;
  if (!createCoreFactory) {
    throw new Error("DesignCore.js loaded but global createCore is unavailable.");
  }

  const wasmResponse = await fetch(designCoreWasmUrl);
  if (!wasmResponse.ok) {
    throw new Error(`DesignCore.wasm load failed: ${wasmResponse.status} ${designCoreWasmUrl}`);
  }
  const wasmBinary = await wasmResponse.arrayBuffer();

  const locateFile =
    options.locateFile ??
    ((path: string) => (path.endsWith(".wasm") ? designCoreWasmUrl : path));

  return createCoreFactory({ ...options, locateFile, wasmBinary });
}

export default createCore;
export * from "./types/core-api";
