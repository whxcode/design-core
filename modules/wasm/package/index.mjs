export const designCoreScriptUrl = new URL("./DesignCore.js", import.meta.url).href;
export const designCoreWasmUrl = new URL("./DesignCore.wasm", import.meta.url).href;

let loadPromise = null;

export function loadDesignCoreScript() {
  if (typeof document === "undefined") {
    return Promise.reject(new Error("loadDesignCoreScript requires a browser document."));
  }

  if (globalThis.createCore) {
    return Promise.resolve();
  }

  if (loadPromise) {
    return loadPromise;
  }

  const existing = document.querySelector('script[data-z-design-core="true"]');
  if (existing) {
    loadPromise = new Promise((resolve, reject) => {
      existing.addEventListener("load", () => resolve(), { once: true });
      existing.addEventListener("error", () => reject(new Error("DesignCore.js load failed")), {
        once: true,
      });
    });
    return loadPromise;
  }

  loadPromise = new Promise((resolve, reject) => {
    const script = document.createElement("script");
    script.src = designCoreScriptUrl;
    script.async = true;
    script.dataset.zDesignCore = "true";
    script.onload = () => resolve();
    script.onerror = () => reject(new Error("DesignCore.js load failed"));
    document.body.appendChild(script);
  });

  return loadPromise;
}

export async function createCore(options = {}) {
  await loadDesignCoreScript();

  const createCoreFactory = globalThis.createCore;
  if (!createCoreFactory) {
    throw new Error("DesignCore.js loaded but global createCore is unavailable.");
  }

  const locateFile =
    options.locateFile ??
    ((path) => (path.endsWith(".wasm") ? designCoreWasmUrl : new URL(path, import.meta.url).href));

  return createCoreFactory({ ...options, locateFile });
}

export default createCore;
