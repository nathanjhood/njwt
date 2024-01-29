/**
 * The 'njwt' C++ addon interface.
 */
interface njwt {
  /**
   * Returns a string, confirming the addon is online.
   * @returns string
   */
  hello(): string;
  /**
   * Returns a number, confirming the addon API version number.
   * @returns number
   */
  version(): number;
}
const njwt: njwt = require('../build/lib/njwt.node');
export = njwt;
