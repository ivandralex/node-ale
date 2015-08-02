var addon = require('bindings')('ale');

console.log("Total reward in js: " + addon.play("/home/andrew/deephack-skynet/roms/seaquest.bin")); // *total reward*