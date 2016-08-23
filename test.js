var syspipe = require('./build/Release/syspipe');

var res = syspipe.pipe();
var dup = syspipe.dup(2)
console.log(dup)

console.log(res);

