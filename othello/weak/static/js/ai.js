(function(global) {
  renew_record();
  ("use strict");
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

  function updatePutable(state) {
    for (var i = 0; i < 64; i++) {
      if (Ai.canPut(state.map, i, state.turn) === true) {
        state.putable[i] = true;
      } else {
        state.putable[i] = false;
      }
    }
  }

  function isEnd(map) {
    for (var i = 0; i < COLXCOL; i++) {
      if (map[i] != 0) {
        return false;
      }
    }
    return true;
  }

  function getScore(map) {
    var score = 0;
    for (var i = 0; i < COLXCOL; i++) {
      if (map[i] < 0) {
        score += -1;
      } else if (map[i] > 0) {
        score += 1;
      }
    }
    return score;
  }
  function getWinner(map) {
    if (!isEnd(map)) {
      return 0;
    } else {
      var score = getScore(map);
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
    var s = "";
    for (let n = arr.length / 4 - 1; n >= 0; --n) {
      var hex = 0;
      var d = 1;
      for (let i = 3; i >= 0; --i) {
        hex += arr[n * 4 + i] * d;
        d *= 2;
      }
      s = hex.toString(16) + s;
    }
    return s;
  }

  function map_to_hex(map) {
    var map_black = Array.apply(null, Array(64)).map(function() {
      return 0;
    });
    var map_white = Array.apply(null, Array(64)).map(function() {
      return 0;
    });
    for (let index = 0; index < map.length; index++) {
      if (map[index] == -1) {
        map_black[index] = 1;
      } else if (map[index] == 1) {
        map_white[index] = 1;
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
  function request(arg) {}

  function renew_record() {
    var win = 0;
    var lose = 0;
    var tie = 0;
    $.ajax({
      url: "https://othello.to-kei.net/api/post.php?type=weak",
      type: "GET"
    })
      // Ajaxリクエストが成功した時発動
      .done(data => {
        var text =
          "最弱AI  " +
          String(data["lose"]) +
          "勝  " +
          String(data["win"]) +
          "敗  " +
          String(data["draw"]) +
          "引分";
        document.getElementById("record").innerHTML = text;
      })
      // Ajaxリクエストが失敗した時発動
      .fail(data => {})
      // Ajaxリクエストが成功・失敗どちらでも発動
      .always(data => {});
  }

  function post_result(result) {
    var JSONdata = {
      source: "weak",
      result: String(result)
    };
    $.ajax({
      url: "https://othello.to-kei.net/api/post.php",
      type: "POST",
      dataType: "json",
      contentType: "application/json",
      data: JSON.stringify(JSONdata)
    })
      // Ajaxリクエストが成功した時発動
      .done(data => {})
      // Ajaxリクエストが失敗した時発動
      .fail(data => {})
      // Ajaxリクエストが成功・失敗どちらでも発動
      .always(data => {});
  }

  function EndProcess(state) {
    console.log("END");
    var score = getScore(state.map);
    var message =
      score < 0
        ? "あなたの" + String(-score) + "石勝ち"
        : score == 0
        ? "引き分け"
        : "あなたの" + String(score) + "石負け";
    var result = score < 0 ? 1 : score == 0 ? 0 : -1;
    post_result(result);
    renew_record();
    document.getElementById("result").innerHTML = message;
    //TODO:リファクタ
    var text;

    switch (result) {
      case 1:
        text = "最弱オセロAIに" + String(-score) + "石で勝った！";
        break;
      case -1:
        text = "最弱オセロAIに" + String(score) + "石で負けた！";
        break;
      case 0:
        text = "最弱オセロAIに引き分けた！";
        break;
    }
    var html =
      '<p id="tweet-share" class="center"><iframe id="twitter-widget-0" scrolling="no" frameborder="0" allowtransparency="true" class="twitter-share-button twitter-share-button-rendered twitter-tweet-button" style="position: static; visibility: visible; width: 61px; height: 20px;" title="Twitter Tweet Button" src="https://platform.twitter.com/widgets/tweet_button.0639d67d95b7680840758b6833f06d87.en.html#dnt=false&amp;hashtags=%E6%9C%80%E5%BC%B1%E3%82%AA%E3%82%BB%E3%83%AD&amp;id=twitter-widget-0&amp;lang=en&amp;original_referer=http%3A%2F%2Fothello.to-kei.net%2Fweak%2F&amp;size=m&amp;text=' +
      text +
      '&amp;type=share&amp;url=http%3A%2F%2Fothello.to-kei.net%2Fweak%2F"></iframe></p>';
    document.getElementById("tweet-share").innerHTML = html;
  }

  function thinkAI(state, ctx, point) {
    // if (Math.random() < 0.2) {
    //   renew_record();
    // }
    document.getElementById("result").innerHTML = "AI思考中";
    $("body").addClass("oclose");
    var arg = map_to_hex(state.map) + turn_to_hex(state.turn);
    var count = 0;
    var result = getWinner(state.map);
    $.ajax({
      url:
        "https://2f7bvxv0w0.execute-api.ap-southeast-1.amazonaws.com/default/reverseproxy2?board=" +
        arg,
      type: "GET",
      timeout: 61000,
      //通信状態に問題がないかどうか
      success: function(res) {
        console.log(res);
        if (res == -1) {
          document.getElementById("result").innerHTML = "パス";
          // state.turn *= -1;
        } else {
          state.map = Ai.putMap(state.map, res, state.turn);
        }
        state.turn *= -1;
        state.revision += 1;
        updatePutable(state);
        Render.render(ctx, state, point);
        // PASS判定
        if (!Ai.hasMove(state.map, state.turn)) {
          state.turn *= -1;
          state.revision += 1;
          updatePutable(state);
          Render.render(ctx, state, point);
          if (Ai.hasMove(state.map, state.turn)) {
            console.log("PASS");
            document.getElementById("result").innerHTML = "パス";
            setTimeout(function() {
              $("body").removeClass("oclose");
              Ai.thinkAI(state, ctx, point);
            }, 20);
          } else {
            $("body").removeClass("oclose");
            EndProcess(state);
          }
        } else {
          $("body").removeClass("oclose");
          document.getElementById("result").innerHTML = "あなたの番";
        }
      },
      //通信エラーになった場合の処理
      error: function(err) {
        document.getElementById("result").innerHTML =
          "サーバーが混雑しています。申し訳ありませんが、ゲームをはじめからやり直してください。";
        if (count > 1) {
          return;
        }
        count += 1;
        request(arg);
      }
    });
  }
})((this || 0).self || global);
