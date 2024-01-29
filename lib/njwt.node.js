const platform = process.platform;
var buildDir = "/build/lib/";

if(platform === "win32")
  buildDir = "\\build\\bin\\Release\\";

const njwt = require(`..${buildDir}njwt.node`);
module.exports = njwt;
