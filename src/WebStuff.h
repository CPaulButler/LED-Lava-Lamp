// ---------------------------------------------------------------------------------------
//
// Code for a simple webserver on the ESP32 (device used for tests: ESP32-WROOM-32D).
// The code generates two random numbers on the ESP32 and uses Websockets to continuously
// update the web-clients. For data transfer JSON encapsulation is used.
//
// For installation, the following libraries need to be installed:
// * Websockets by Markus Sattler (can be tricky to find -> search for "Arduino Websockets"
// * ArduinoJson by Benoit Blanchon
//
// NOTE: in principle this code is universal and can be used on Arduino AVR as well. However, AVR is only supported with version 1.3 of the webSocketsServer. Also, the Websocket
// library will require quite a bit of memory, so wont load on Arduino UNO for instance. The ESP32 and ESP8266 are cheap and powerful, so use of this platform is recommended.
//
// Refer to https://youtu.be/15X0WvGaVg8
//
// Written by mo thunderz (last update: 27.08.2022)
//
// ---------------------------------------------------------------------------------------

#include <WiFi.h>                                     // needed to connect to WiFi
#include <WebServer.h>                                // needed to create a simple webserver (make sure tools -> board is set to ESP32, otherwise you will get a "WebServer.h: No such file or directory" error)
#include <WebSocketsServer.h>                         // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>                              // needed for JSON encapsulation (send multiple variables with one string)
#include <FastLED.h>
#include "modes.h"


// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
String webpage = R"(
<!DOCTYPE html>
<html>
<head>
<title>LED Strip Controller</title>
</head>
<body style='background-color: #EEEEEE;'>

<span style='color: #003366;'>

<h1>Change LED Strip Pattern</h1>
<p><button type='button' id='Off_Button'> Off </button></p>
<p><button type='button' id='Rainbow_Button'> Rainbow </button></p>
<p><button type='button' id='Comet_Button'> Comet </button></p>
<p><button type='button' id='Spring_Button'> Spring </button></p>

</span>

</body>
<script>
  var Socket;
  document.getElementById('Off_Button').addEventListener('click', function(){button_send_back('off')});
  document.getElementById('Rainbow_Button').addEventListener('click', function(){button_send_back('rainbow')});
  document.getElementById('Comet_Button').addEventListener('click', function(){button_send_back('comet')});
  document.getElementById('Spring_Button').addEventListener('click', function(){button_send_back('spring')});

  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event) {
      processCommand(event);
    };
  }
  function button_send_back(button) {
    var msg = {}
    msg['type'] = button;
    var msgStr = JSON.stringify(msg);
	Socket.send(msgStr);
  }
  function processCommand(event) {
	var obj = JSON.parse(event.data);
	document.getElementById('rand1').innerHTML = obj.rand1;
	document.getElementById('rand2').innerHTML = obj.rand2;
    console.log(obj.rand1);
	console.log(obj.rand2);
  }
  window.onload = function(event) {
    init();
  }
</script>
</html>
)";


void (*webSetPattern) (int pattern);

// The JSON library uses static memory, so this will need to be allocated:
// -> in the video I used global variables for "doc_tx" and "doc_rx", however, I now changed this in the code to local variables instead "doc" -> Arduino documentation recommends to use local containers instead of global to prevent data corruption

// We want to periodically send values to the clients, so we need to define an "interval" and remember the last time we sent data to the client (with "previousMillis")
int interval = 1000;                                  // send data to the client every 1000ms -> 1s
unsigned long previousMillis = 0;                     // we use the "millis()" command for time reference and this will output an unsigned long

// Initialization of webserver and websocket
WebServer server(80);                                 // the server uses port 80 (standard port for websites
WebSocketsServer webSocket = WebSocketsServer(81);    // the websocket uses port 81 (standard port for websockets

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      // try to decipher the JSON string received
      JsonDocument doc;                    // create a JSON container
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {
        // JSON string was received correctly, so information can be retrieved:
        const char* g_type = doc["type"];
        Serial.println("Type: " + String(g_type));
        if (String(g_type).equals("off")){
            Serial.println("calling off");
            webSetPattern(Off);
        }
        if (String(g_type).equals("comet")){
            Serial.println("calling comet");
            webSetPattern(Comet);
        }
        if (String(g_type).equals("rainbow")) {
            Serial.println("calling rainbow");
            webSetPattern(Rainbow);
        }
        if (String(g_type).equals("spring")) {
            Serial.println("calling spring");
            webSetPattern(Spring);
        }

        FastLED.clear();
      }
      Serial.println("");
      break;
  }
}

void startAccessPoint(){
  // SSID and password of Wifi connection:
  const char* ssid = "LED Strip";

  // Configure IP addresses of the local access point
  IPAddress local_IP(192,168,1,22);
  IPAddress gateway(192,168,1,5);
  IPAddress subnet(255,255,255,0);

  Serial.print("Setting up Access Point ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Starting Access Point ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");

  Serial.print("IP address = ");
  Serial.println(WiFi.softAPIP());
}

void connectRouter(){
  // SSID and password of Wifi connection:
  const char* ssid = "Nacho Wi-Fi";
  const char* password = "Alpha Beta Souper";

  WiFi.begin(ssid, password);                         // start WiFi interface
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));     // print SSID to the serial interface for debugging

  while (WiFi.status() != WL_CONNECTED) {             // wait until WiFi is connected
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());                     // show IP address that the ESP32 has received from router
}

void webSetup(void (SetPatternFunc)(int pattern)) {

  connectRouter();
  //startAccessPoint();

  webSetPattern = SetPatternFunc;

  server.on("/", []() {                               // define here wat the webserver needs to do
    server.send(200, "text/html", webpage);           //    -> it needs to send out the HTML string "webpage" to the client
  });
  server.begin();                                     // start server

  webSocket.begin();                                  // start websocket
  webSocket.onEvent(webSocketEvent);                  // define a callback function -> what does the ESP32 need to do when an event from the websocket is received? -> run function "webSocketEvent()"
}

void webLoop() {
  server.handleClient();                              // Needed for the webserver to handle all clients
  webSocket.loop();                                   // Update function for the webSockets

  unsigned long now = millis();                       // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) > interval) { // check if "interval" ms has passed since last time the clients were updated

    String jsonString = "";                           // create a JSON string for sending data to the client
    JsonDocument doc;                      // create a JSON container
    doc["rand1"] = random(100);                    // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    doc["rand2"] = random(100);
    serializeJson(doc, jsonString);                   // convert JSON object to string
    // Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString);               // send JSON string to clients

    previousMillis = now;                             // reset previousMillis
  }
}
