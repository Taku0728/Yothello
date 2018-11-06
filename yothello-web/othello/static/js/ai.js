(function(global) {
  "use strict";
  // Class ------------------------------------------------
  function Ai() {}

  // Header -----------------------------------------------
  global.Ai = Ai;
  global.Ai.thinkAI = thinkAI;
  global.Ai.canPut = canPut;
  global.Ai.getNodeList = getNodeList;
  global.Ai.putMap = putMap;
  global.Ai.hasMove = hasMove;

  //-------------------------------------
  var COL = 8;
  var COLXCOL = COL * COL;

  function getEffectArray(map, number, turn) {
    var list = [];
    var x = number % COL | 0;
    var y = (number / COL) | 0;
    var target = [];
    if (map[number] != 0) {
      return list;
    }
    for (var x_inc = -1; x_inc <= 1; x_inc++) {
      for (var y_inc = -1; y_inc <= 1; y_inc++) {
        if (x_inc === 0 && y_inc === 0) {
          continue;
        }
        target = [];
        L: for (
          var _x = x + x_inc, _y = y + y_inc;
          (_x >= 0) & (_y >= 0) && _x < COL && _y < COL;
          _x = _x + x_inc, _y = _y + y_inc
        ) {
          if (map[_y * COL + _x] * turn > 0) {
            list = list.concat(target);
            break L;
          } else if (map[_y * COL + _x] == 0) {
            break L;
          } else {
            target.push(_y * COL + _x);
          }
        }
      }
    }
    return list;
  }

  function canPut(map, number, turn) {
    var x = number % COL | 0;
    var y = (number / COL) | 0;
    var target = [];
    if (map[number] != 0) {
      return false;
    }
    for (var x_inc = -1; x_inc <= 1; x_inc++) {
      for (var y_inc = -1; y_inc <= 1; y_inc++) {
        if (x_inc === 0 && y_inc === 0) {
          continue;
        }
        target = [];
        L: for (
          var _x = x + x_inc, _y = y + y_inc;
          (_x >= 0) & (_y >= 0) && _x < COL && _y < COL;
          _x = _x + x_inc, _y = _y + y_inc
        ) {
          if (map[_y * COL + _x] * turn > 0) {
            if (target.length > 0) {
              return true;
            }
            break L;
          } else if (map[_y * COL + _x] == 0) {
            break L;
          } else {
            target.push(_y * COL + _x);
          }
        }
      }
    }
    return false;
  }

  function hasMove(map, turn) {
    var flag = false;
    for (let i = 0; i < 64; ++i) {
      if (canPut(map, i, turn)) {
        flag = true;
        break;
      }
    }
    return flag;
  }

  function getNodeList(map, turn) {
    var node_list = [];
    for (var i = 0; i < COLXCOL; i++) {
      if (canPut(map, i, turn)) {
        node_list.push(i);
      }
    }
    return node_list;
  }

  function putMap(map, number, turn) {
    var effectArray = getEffectArray(map, number, turn);
    var _map = map.concat();
    _map[number] = turn;
    for (var i = 0; i < effectArray.length; i++) {
      var _number = effectArray[i] | 0;
      switch (_map[_number]) {
        case 1:
          _map[_number] = -1;
          break;
        case -1:
          _map[_number] = +1;
          break;
      }
    }
    return _map;
  }

  function evalMap(map) {
    var ev = 0;
    for (var i = 0; i < COLXCOL; i++) {
      switch (map[i]) {
        case 1:
          ev += 1;
          break;
        case -1:
          ev += -1;
          break;
      }
    }
    return ev;
  }

  function isEnd(map) {
    for (var i = 0; i < COLXCOL; i++) {
      if (map[i] != 0) {
        return false;
      }
    }
    return true;
  }

  function getWinner(map) {
    var score = 0;
    if (!isEnd(map)) {
      return 0;
    } else {
      for (var i = 0; i < COLXCOL; i++) {
        if (map[i] < 0) {
          score += -1;
        } else if (map[i] > 0) {
          score += 1;
        }
      }
      if (score > 0) {
        return 1;
      } else if (score < 0) {
        return -1;
      } else {
        return 0;
      }
    }
  }

  function bi_to_hex(arr) {
    var hex = 0;
    var d = 1;
    for (let i = arr.length - 1; i >= 0; --i) {
      hex += arr[i] * d;
      d *= 2;
    }
    return hex.toString(16);
  }

  function map_to_hex(map) {
    var map_black = Array(64);
    map_black = map_black.map(function() {
      return 0;
    });
    var map_white = Array(64);
    map_white = map_white.map(function() {
      return 0;
    });
    for (let index = 0; index < map.length; index++) {
      if (map[index] == -1) {
        map_black = 1;
      } else if (map[index] == 1) {
        map_white = 1;
      }
    }
    var hex_black = bi_to_hex(map_black);
    var hex_white = bi_to_hex(map_white);
    var zeros = "0000000000000000";
    return (zeros + hex_black).slice(-16) + (zeros + hex_white).slice(-16);
  }

  function turn_to_hex(turn) {
    var hex = 1;
    if (turn == 1) {
      hex = 2;
    }
    return hex;
  }

  function thinkAI(map, turn_player) {
    var arg = map_to_hex(map) + turn_to_hex(turn_player);
    return Math.floor(Math.random() * 65);
  }
})((this || 0).self || global);
