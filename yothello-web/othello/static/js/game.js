(function(global) {
  "use strict";
  // Class ------------------------------------------------
  function Game() {}

  // Header -----------------------------------------------
  global.Game = Game;
  global.Game.initGame = initGame;

  // ------------------------------------------------------
  var COL = 8;
  var ctx;
  var evented = false;
  var state = {};
  var point = {
    x: 0,
    y: 0
  };
  var init_state = {
    map: [
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      1,
      -1,
      0,
      0,
      0,
      0,
      0,
      0,
      -1,
      1,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    ],
    mode: 0,
    turn: -1,
    revision: 0,
    selected: {
      name: "",
      value: 0
    }
  };
  var supportTouch = "ontouchend" in document;
  var EVENTNAME_TOUCHSTART = supportTouch ? "touchstart" : "mousedown";
  var EVENTNAME_TOUCHMOVE = supportTouch ? "touchmove" : "mousemove";
  var EVENTNAME_TOUCHEND = supportTouch ? "touchend" : "mouseup";
  function initGame(_ctx) {
    ctx = _ctx;
    state = objCopy(init_state);
    if (!evented) {
      evented = true;
      setEvents();
    }
    Render.render(ctx, state, point);
  }

  function setEvents() {
    var isTouch;
    if ("ontouchstart" in window) {
      isTouch = true;
    } else {
      isTouch = false;
    }
    if (isTouch) {
      ctx.canvas.addEventListener(EVENTNAME_TOUCHSTART, ev_mouseClick);
    } else {
      window.addEventListener(EVENTNAME_TOUCHMOVE, ev_mouseMove);
      ctx.canvas.addEventListener(EVENTNAME_TOUCHEND, ev_mouseClick);
    }
  }

  function ev_mouseMove(e) {
    updateXY(e);
    state.selected = hitTest(point.x, point.y);
    Render.render(ctx, state, point);
  }

  function ev_mouseClick(e) {
    updateXY(e);
    var selected = hitTest(point.x, point.y);
    var number;
    if (selected.name === "RECT_BOARD") {
      number = selected.value;
      if (Ai.canPut(state.map, selected.value, state.turn) === true) {
        var end = false;
        if (!end) {
          state.map = Ai.putMap(state.map, selected.value, state.turn);
          state.turn = -1 * state.turn;
          state.revision += 1;
          Render.render(ctx, state, point);

          setTimeout(function() {
            Ai.thinkAI(state, ctx, point);
          }, 100);
        }
      }
    }
  }

  var updateXY = function(e) {
    var original = e.originalEvent;
    var rect = e.target.getBoundingClientRect();
    if (e.touches) {
      console.log("touch");
      point.x = e.touches[0].pageX - rect.left;
      point.y = e.touches[0].pageY - rect.top;
    } else {
      point.x = e.pageX - rect.left;
      point.y = e.pageY - rect.top;
    }
  };

  function hitTest(x, y) {
    var objects = [Render.RECT_BOARD];
    var click_obj = null;
    var selected = {
      name: "",
      value: 0
    };
    for (var i = 0; i < objects.length; i++) {
      if (
        objects[i].w >= x &&
        objects[i].x <= x &&
        objects[i].h >= y &&
        objects[i].y <= y
      ) {
        selected.name = "RECT_BOARD";
        break;
      }
    }
    switch (true) {
      case selected.name === "RECT_BOARD":
        selected.name = "RECT_BOARD";
        selected.value =
          Math.floor(y / Render.CELL_SIZE) * COL +
          Math.floor(x / Render.CELL_SIZE);
        break;
    }
    return selected;
  }

  function objCopy(obj) {
    return JSON.parse(JSON.stringify(obj));
  }
})((this || 0).self || global);
