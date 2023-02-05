// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.
const createTimer = require("../count-down-timer");

//change timer values here
const timer = createTimer(3, 20);
const $mins = document.querySelector("#mins");
const $secs = document.querySelector("#secs");

function updateVal() {

  let currMins = timer.getMins();
  let currSecs = timer.getSecs();
  if(currMins == 0 && currSecs == 0 ){
    updateStartBtn(true);
  }
  if(currMins >= 0 && currMins < 10){
    currMins = currMins.toString().padStart(2, '0');
  }
  if(currSecs >= 0 && currSecs < 10){
    currSecs = currSecs.toString().padStart(2, '0');
  }
  $mins.innerHTML = currMins;
  $secs.innerHTML = currSecs;
}

const $startBtn = document.querySelector("#start");
const $stopBtn = document.querySelector("#stop");
const $resetBtn = document.querySelector("#reset");
const $add5Btn = document.querySelector("#add5");
const $sub5Btn = document.querySelector("#sub5");

$startBtn.addEventListener("click", () => {
  timer.start(timer.start(function () {
    console.log("test start timer", timer, getMins(), timer.getSecs());
  }));
  const started = timer.isStarted();
  updateStartBtn(started);
  updateVal();
});

$stopBtn.addEventListener("click", () => {
  timer.stop();
  const started = timer.isStarted();
  updateStartBtn(started);
});

$resetBtn.addEventListener("click", () => {
  timer.reset();
  updateVal();
});

$add5Btn.addEventListener("click", () => {
  timer.addSecs(5);
  updateVal();
});

$sub5Btn.addEventListener("click", () => {
  timer.subSecs(5);
  const started = timer.isStarted();
  updateStartBtn(started);
  updateVal();
});

function updateStartBtn(started){
  $startBtn.disabled = started;
  $stopBtn.disabled = !started;
}

updateVal();
updateStartBtn(false);
