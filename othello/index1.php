<!DOCTYPE html>
<html>
<head>
  <!-- Global site tag (gtag.js) - Google Analytics -->
  <script async src="https://www.googletagmanager.com/gtag/js?id=UA-144862184-1"></script>
  <script>
    window.dataLayer = window.dataLayer || [];
    function gtag(){dataLayer.push(arguments);}
    gtag('js', new Date());

    gtag('config', 'UA-144862184-1');
  </script>
  <meta content="text/html" charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <link rel="stylesheet" type="text/css" href="https://othello.to-kei.net/static/style.css">
  <link rel="stylesheet" type="text/css" href="https://othello.to-kei.net/static/style_s.css">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!--<meta name="viewport" content="width=device-width, initial-scale=1.0" />-->
  <title>強いオセロ - AVILEN</title>
  <link rel="icon" href="https://to-kei.net/wp-content/uploads/2017/06/cropped-IMG_0190-32x32.png" sizes="32x32" />
</head>

<body>
  <div id="contents">
    <p id="record" class="center" style="margin-top:5px;margin-bottom:5px">強いAI 96579勝 1877敗 41引分</p>
    <!--<p class="center" style="margin-top:-15px"><font size=1>＊ただいまメンテナンス中のため戦績更新はありません</font></p>-->
    <canvas id="canv"></canvas>
    <p id="result" class="center">あなたの番</p>
    <p id="tweet-share" class="center"></p>
    <p style="text-align:left;margin-bottom:-10px;float:left;margin-left:4px">
      <a href="https://othello.to-kei.net/weak"><font size=3 color="blue">最弱オセロ</font></a></p>
    <p style="text-align:right;margin-bottom:-10px;"> 
    　　<a><font size=3>強いオセロ</font></a></p>
    <hr>
    <div style="text-align: center">
      <p> <font size=2>開発者 - </font>
      <a href="https://twitter.com/TakuAtCoder1"><font size=2 color="blue">吉田 拓真</font></font></a></p>
      <p> <font size=2><a href="https://avilen.co.jp"><font color="blue">株式会社AVILEN </a></font> - CTO</font></p>
      <p><a href="https://to-kei.net/"><font size=2 color="blue">全人類がわかる統計学</font></a> </p>
      <br>
      <picture>
      <source srcset="/static/img/white.png">
        <img class="" src="/static/img/white.png" width="250px" alt="株式会社AVILEN">
      </picture>
      <br>
    </div>
  </div>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
  <script src="static/js/render.js"></script>
  <script src="static/js/ai.js"></script>
  <script src="static/js/game.js"></script>
  <script>

  </script>
  <script>
  var w = $('#contents').width();
  $('#canv').attr('width', w);
  $('#canv').attr('height', w);

  var ctx = document.getElementById("canv").getContext('2d');
  Game.initGame(ctx);
  </script>
  <div class="mask"><p class="mask-p">オセロAIが解析中です</p></div>
</body>
