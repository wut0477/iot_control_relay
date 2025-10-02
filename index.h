const char index_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="Content-Language" content="th">
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <meta http-equiv="content-Type" content="text/html; charset=window-874">
  <meta http-equiv="content-Type" content="text/html; charset=tis-620">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 2.0rem; }
    p { font-size: 2.0rem; }
    i { font-size: 2.0rem; }
    .units { font-size: 1.5rem; }
    .dht-labels{
      font-size: 2.0rem;
      vertical-align:middle;
      padding-bottom: 15px;
    
    }
    .data_value{
        font-size: 2.0rem;
    }
    .setup_data{
      font-size: 2.0rem;
    }
  </style>
</head>
<body  style="background-color:powderblue;">
  <h2>Relay Control With ESP8266 and DHT 11</h2>
  <h2>Asynchronous Web Server</h2> 
<hr>


  <form action="/action_page">
    <div class="data_value">Temperature Setting:</div>
    <div class=""><input type="text" class="setup_data" name="value_setting" value=""></div>
    <div class=""><input class=" btn btn-primary" type="submit" value="Send Data"></div>
  </form> 
  

<hr>

 </br></br>
 <div class="container">
    <div class="row">
      <div class="col-md-6 ">
       <i class="fas fa-sign-out-alt" style="color:blue"></i>
        <span class="data_value">Status :</span>
        <span id="status" class="data_value">%STATUS%</span>   
      </div> 
      <div class="col-md-6 ">
        <i class="fas fa-cog" style="color:blue"></i> 
        <span class="data_value">Value Set :</span>
        <span class="btn btn-primary data_value">%TEMPSET%</span >
      </div>
    </div>
  </div>
 
 <hr>
<div class="container">
    <div class="row">
      <div class="col-md-6 ">
        <i class="fas fa-thermometer-half" style="color:crimson"></i> 
        <span class="dht-labels">อุณหภูมิ:</span>
        <span id="temperature" class="data_value btn btn-danger">%TEMPERATURE%</span>
        <sup class="units">&deg;C</sup>
        </span>
      </div> 
      <div class="col-md-6 ">
        <i class="fas fa-tint" style="color:green"></i> 
        <span class="dht-labels">ความชื้น:</span>
        <span id="humidity" class="data_value btn btn-success">%HUMIDITY%</span>
        <sup class="units">%</sup>
      </div>
    </div>
  </div>
<br>
<hr>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("status").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/status", true);
  xhttp.send();
}, 1000 ) ;


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temp_set").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temp_set", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)=====";
