
#include <ESP8266WiFi.h>
#include "configure.h"

const char* ssid     = "xxxx"; // your WiFi SSID
const char* password = "xxxx"; // your WiFi Password
const char* host = SERVER_IP; // the IP address of the server running xSchedule
short pressed[100]; // an array keeping state of the buttons between loops
short buttons; // number of buttons
int Processed = 0; //for debugging

void setup(){
// Prepare to send debug messages  
#ifdef DEBUG
  Serial.begin(SERIALRATE); delay(10);
#endif

  // work out the number of buttons
  buttons = BUTTONS;
  if (buttons > sizeof(pressed) / sizeof(short))
  {
    buttons = sizeof(pressed) / sizeof(short);
#ifdef DEBUG
    Serial.print("Buttons limited to ");
    Serial.println(buttons);
#endif
  }

  // setup the power pin
  pinMode(POWERPIN, OUTPUT);
  digitalWrite(POWERPIN, HIGH);

  // set up the button pressed pin
  pinMode(PRESSPIN, OUTPUT);

  // setup our input pins
  for (int i = 0; i < buttons; i++)
  {
    pinMode(pins[i], INPUT_PULLUP);
    pressed[i] = LOW;
  }

  // We start by connecting to a WiFi network
#ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  // Next, we configure the network connection, making sure the WiFi is in Station mode and not AP+Station
  // The IP address is configured manually: 3-5 octet groups of localIP, gateway, subnet, and then optional DNS server addresses (up to 2)
  WiFi.mode(WIFI_STA); 
  WiFi.config(IPAddress(192, 168, 1, 222), IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 0));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

void loop() {

  //Serial.println(digitalRead(12));
  bool ispressed = false; // keep track if any button is pressed so the press indicator LED can be turned on

  // check each button
  for (int i = 0; i < BUTTONS; i++)
  {
    // read it
    short v = digitalRead(pins[i]);

    // if it is pressed immediately light the led
    if (v == LOW)
    {
      digitalWrite(PRESSPIN, HIGH);
      ispressed = true;
    }

    // if the button has changed state
    if (pressed[i] != v)
    {
#ifdef DEBUG
      Serial.print("Pin changed state: ");
      Serial.print(pins[i]);
      Serial.print(" : ");
      Serial.println(v);
#endif
  
  // For every button you have installed and configured to work with xSchedule, you must have an "if" statement with the correct "SendData" parameter(s) below.
  // Make sure both the pin number and the button name are correct.
  
      if (v == LOW)
      {
        // button is newly pressed
        if (pins[i] == 12) { //Process GPIO 12
          SendData("/xScheduleCommand?Command=PressButton&Parameters=Next"); //This xScheduler button is named Next
        }
        if (pins[i] == 14) { //Process GPIO 14
          SendData ("/xScheduleCommand?Command=PressButton&Parameters=Back");  //This xScheduler button is named Back
        }
        if (pins[i] == 13) { //Process GPIO 13
          SendData ("/xScheduleCommand?Command=PressButton&Parameters=Other");  //This xScheduler button is named Other
        }
      }
      pressed[i] = v;
    }
  }

  // if no button is pressed clear the pressed LED
  if (!ispressed)
  {
    digitalWrite(PRESSPIN, LOW);
  }

  // a small delay helps deal with key bounce
  delay(10);

}

void SendData (String urlrequest) {

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  String weburl = urlrequest;
  const int httpPort = WEBPORT;

#ifdef DEBUG
  Serial.println(host + httpPort );
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  Serial.println(weburl);
#endif

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET ") + weburl + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  //json response if no failures:
  //{"result":"ok","reference":"","command":"PressButton"}
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    int hsdev = line.indexOf('ok');
    if (hsdev > -1) {
      Processed = 1;
#ifdef DEBUG
      Serial.println("");
      Serial.println("Correct response received");
      Serial.println("");
      Serial.println("Button press sucessfully sent");
#endif
    } else {
      Processed = 0;
#ifdef DEBUG
      Serial.println("");
      Serial.println("Button was not sucessful");
#endif
    }
  }

#ifdef DEBUG
  Serial.println();
  Serial.println("closing connection");
#endif

}
