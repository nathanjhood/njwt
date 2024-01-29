function test_version() {

  let status = false;

  try {

    const njwt = require("../lib/njwt.node");

    console.log(`Napi Version: ${njwt.version()}`);

    status = true;

  } catch(e) {

    console.log(`${e}`);
  }

  return status;
};

const res_test_version = test_version();

if((!res_test_version))
{
  console.log("'test_version()' failed.");
  return false;
}

console.log("'test_version()' passed.");
return true;
