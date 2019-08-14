<?php
ini_set('display_errors', 1);
$host = "mysql137.phy.lolipop.lan";
$username = "LAA0741421";
$password = "othellotakuma";
$dbname = "LAA0741421-othello";
$mysqli = new mysqli($host, $username, $password, $dbname);
if ($mysqli->connect_error) {
   error_log($mysqli->connect_error);
   exit;
}

header('Content-Type: application/json; charset=utf-8');

$contentType = isset($_SERVER['CONTENT_TYPE']) ? $_SERVER['CONTENT_TYPE'] : null;
$requestMethod = isset($_SERVER['REQUEST_METHOD']) ? $_SERVER['REQUEST_METHOD'] : null;

if ($requestMethod == 'GET'){
  $type = $_GET["type"];
  $l = check_reuslt($type);
  $win = $l[0];
  $lose = $l[1];
  $draw = $l[2];
  sendSuccess($win,$lose,$draw);
}
elseif($requestMethod !== 'POST' || $contentType !== 'application/json') {
    sendClientError(400, 'bad request');
}

$data = json_decode(file_get_contents('php://input'), true);
if ($data === null) {
    sendClientError(400, 'json parse error');
}

try{
  $type = $data["source"];
  $result = $data["result"];
}
catch (Exception $e) {
  sendServerError("500","json parse error");
}



$l = check_reuslt($type);
$win = $l[0];
$lose = $l[1];
$draw = $l[2];
if($result == 0){
  $draw = $draw + 1;
}
elseif($result == 1){
  $win = $win + 1;
}
elseif($result == -1){
  $lose = $lose + 1;
}
update_record($win,$lose,$draw,$type);
sendSuccess($win,$lose,$draw);

function sendClientError($status, $message) {
    http_response_code($status);
    echo json_encode([
        'status' => 'fail',
        'message' => $message,
    ]);
    exit;
}

function sendServerError($status, $message) {
    http_response_code($status);
    echo json_encode([
        'status' => 'error',
        'message' => $message,
    ]);
    exit;
}

function sendSuccess($win,$lose,$draw) {
    http_response_code(200);
    echo json_encode([
        'status' => 'success',
        'win' => $win,
        'lose' => $lose,
        'draw' => $draw
    ]);
    exit;
}

function check_reuslt($type){
  global $mysqli;
  $query = "SELECT * FROM result WHERE type='$type'";
  $result = $mysqli->query($query);
  while ($row = $result->fetch_assoc()) {
    $win   = $row['win'];
    $lose  = $row['lose'];
    $draw = $row['draw'];
  }
  return array($win, $lose, $draw);
}


function update_record($win, $lose,$draw, $type){
  global $mysqli;
  $query = "UPDATE result SET win=$win, lose =$lose ,draw=$draw WHERE type='$type'";
  $result = $mysqli->query($query);
}
