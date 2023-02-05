var createTimer = require('bindings')('addon');

var timer = createTimer(0,10);
// console.log( timer );
console.log( timer.start(function () {
    console.log("test", timer,getMins(), timer.getSecs());
}) ); // 11
// console.log( timer.subSecs(6) );
// console.log( timer.addSecs(2) );
// console.log( timer.reset() );
// console.log( timer.stop() );
// console.log( timer.isStarted() );