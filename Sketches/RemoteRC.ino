
#include <ESP8266WiFi.h>
 
const char* ssid = "AndroidAp";
const char* password = "stanbuzz1";
 
WiFiServer server(80);
int ioA = 12; // D6
int ioB = 13; // D7
int ioC = 15; // D8
int a,b,c;
String btnToBlur = "";
String request;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Initially Stop the Car
  a = LOW;
  b = LOW;
  c = LOW;
 
  pinMode(ioA, OUTPUT);
  pinMode(ioB, OUTPUT);
  pinMode(ioC, OUTPUT);
  digitalWrite(ioA, a);
  digitalWrite(ioB, b);
  digitalWrite(ioC, c);
 
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  // Check if a client has connected
  client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.print(" { ");
  while(!client.available()){
    delay(1);
  }

  readReq(); // Read the client request
  
  processReq(); // Perform action on request
  
  delay(1);
  Serial.println(" } ");
  Serial.println("");
 
}

void readReq() {
  // Read the first line of the request
  request = client.readStringUntil('\r');
//  Serial.println(request);
  client.flush();
 
  // Match the request
 
  if (request.indexOf("/RC=FW") != -1)  {
    a = HIGH;
    b = HIGH;
    c = HIGH;
    Serial.print("Move Forward");
    btnToBlur="f";
  }
 
  if (request.indexOf("/RC=LT") != -1)  {
    a = HIGH;
    b = LOW;
    c = LOW;
    Serial.print("Move Left");
    btnToBlur="l";
  }
 
  if (request.indexOf("/RC=ST") != -1)  {
    a = LOW;
    b = LOW;
    c = LOW;
    Serial.print("Move Stop");
    btnToBlur="s";
  }
 
  if (request.indexOf("/RC=RT") != -1)  {
    a = LOW;
    b = HIGH;
    c = LOW;
    Serial.print("Move Right");
    btnToBlur="r";
  }
 
  if (request.indexOf("/RC=BK") != -1)  {
    a = HIGH;
    b = HIGH;
    c = LOW;
    Serial.print("Move Backward");
    btnToBlur="b";
  }

}

void processReq() { 
  // Set all Pin
  digitalWrite(ioA,a);
  digitalWrite(ioB,b);
  digitalWrite(ioC,c);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.print("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"user-scalable=no,maximum-scale=1\"><style>body{width:100%; padding:0; margin:0;}.bx{text-align:center; padding:25% 2px;}.row{}.w25{width:25% !important;}.w50{width:49% !important;}.w75{width:75% !important;}.w100{width:100% !important;}.btn{min-height:20vw; border-radius:20em; font-size:8em;}.row .btn.vb{padding:.5em 0;}.bt-");
  client.print(btnToBlur);
  client.println("{background-color:#e6e6e6 !important;color:#fff !important;}.gyB{font-size:2.4em;border-radius:90%;padding:.5em;margin:24px;position:absolute;top:0;right:0;border:4px solid;background-color:#ffffff;color:#2196f3;outline:0;z-index:1;}.gyB:hover{color:#ffffff;background-color:#2196f3;}.arr{color:#4CAF50;top:0;bottom:0;right:0;left:0;margin:auto;z-index:10;position:absolute;font-size:12em;font-family:monospace;height:60vw;width:60vw;}.a-up{transform:rotate(270deg);position:relative;top:-22vw;left:25vw;}.a-bt{transform:rotate(90deg);position:relative;bottom:-28vw;left:10vw;}.a-lt{transform:rotate(180deg);left:0vw;top:5vw;position:absolute;}.a-rt{float:right;top:3vw;right:0;position:absolute;}.ar{display:inline-block;}.gy{display:none;padding:0;margin:0;background:#efebe4;width:100%;height:100%;position:absolute;top:0;left:0;}.line{position:absolute;width:5px;height:3px;margin:auto;left:0px;right:0px;top:0px;bottom:0px;background:black;-webkit-transform-origin:0% 0%;transform-origin:0% 0%; z-index:100;}#x{}#y{width:3px;height:5px;}#angle{position:absolute;width:60vw;height:60vw;border:5px solid #673AB7;margin:auto;left:0;right:0;top:0;bottom:0;border-radius:50%;-webkit-transform-origin:50% 50%;transform-origin:50% 50%;z-index:100;}#angle:before{content:\"\";position:relative;left:100px;width:20px;height:30px;border:1px solid black;}#angle:after{content:\"\";position:absolute;background:black;left:96px;top:96px;border-radius:50%;width:6px;height:6px;border:1px solid black;}#guideX{position:absolute;top:0px;left:-1px;width:50%;height:100%;border-right:0px solid red;#guideY{position:absolute;width:100%;height:50%;top:-1px;border-bottom:0px solid green;}</style><script>var e,xhttp=new XMLHttpRequest();function toggleGyro(){e=document.getElementsByClassName(\"gy\")[0];if(e.style.display==\"block\")e.style.display=\"none\";else e.style.display=\"block\";}function init(){if(window.DeviceOrientationEvent){window.addEventListener(\"deviceorientation\", function(event){document.getElementById(\"y\").style.webkitTransform=\"scaleY(\"+(Math.round(event.beta)*3)+\")\";document.getElementById(\"x\").style.webkitTransform=\"scaleX(\"+(Math.round(event.gamma)*3)+\")\";document.getElementById(\"angle\").style.webkitTransform=\"rotateZ(\"+(Math.round(event.alpha))+\"deg)\";document.getElementById(\"uu\").innerHTML=Math.round(event.beta)+\" , \"+Math.round(event.gamma);var ar=document.getElementsByClassName(\"ar\");for(var i=0;i<4;i++)ar[i].style.opacity=0;if(Math.round(event.beta)>=20){xhttp.open(\"GET\", \"/RC=RT\", true);ar[1].style.opacity=1;}else if(Math.round(event.beta)<=-20){xhttp.open(\"GET\", \"/RC=LT\", true);ar[0].style.opacity=1;}else if(Math.round(event.gamma)>=20){xhttp.open(\"GET\", \"/RC=FW\", true);ar[3].style.opacity=1;}else if(Math.round(event.gamma)<=-20){xhttp.open(\"GET\", \"/RC=BK\", true);ar[2].style.opacity=1;}else{xhttp.open(\"GET\", \"/RC=ST\",true);}if(e.style.display==\"block\")xhttp.send();},true);}else{alert(\"Sorry, your browser does not support Device Orientation\");}}</script></head><body onload=\"init()\"><button class=\"gyB\" onclick=\"toggleGyro()\">Gyro<br>Mode</button><div class=\"bx\"><div class=\"row\"><a href=\"/RC=FW\"><button class=\"btn bt-f w75\" style=\"background-color:rgb(139,195,74)\">FRWD</button></a></div><div class=\"row\"><a href=\"/RC=LT\"><button class=\"btn vb bt-l w25\" style=\"background-color:rgb(255,152,0)\">L<br>F<br>T</button></a><a href=\"/RC=ST\"><button class=\"btn bt-s w50\" style=\"background-color:rgb(244,67,54)\"><br>STOP<br><br></button></a><a href=\"/RC=RT\"><button class=\"btn bt-r vb w25\" style=\"background-color:rgb(255,193,7)\">R<br>G<br>T</button></a></div><div class=\"row\"><a href=\"/RC=BK\"><button class=\"btn bt-b w75\" style=\"background-color:rgb(96,125,139)\">BACK</button></a></div></div><div class=\"gy\"><div id=\"x\" class=\"line\"></div><div id=\"y\" class =\"line\"></div><div id=\"angle\"></div><div class=\"arr\"><p class=\"ar a-up\">></p><p class=\"ar a-bt\">></p><p class=\"ar a-lt\">></p><p class=\"ar a-rt\">></p></div><div id=\"guideX\"></div><div id=\"guideY\"></div><p id=\"uu\"></p></div></body></html>");
}

