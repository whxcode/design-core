const createCoreFactory = require("./DesignCore.js");

function createCore(options = {}) {
  const locateFile =
    options.locateFile ??
    ((filePath) => {
      if (typeof document !== "undefined" && document.currentScript?.src) {
        return new URL(filePath, document.currentScript.src).href;
      }

      return filePath;
    });

  return createCoreFactory({ ...options, locateFile });
}

module.exports = createCore;
module.exports.default = createCore;
module.exports.createCore = createCore;
module.exports.createCoreFactory = createCoreFactory;
