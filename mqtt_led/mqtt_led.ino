/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 62
// Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.

const char* ssid = "WAIFAI_V4A3";
const char* password = "24419083V";
const char* mqtt_server = "192.168.1.125";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
byte scene_active = 0;
void callback(char* topic, byte* message, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, message, length);
serializeJson(doc, Serial);
  if(doc["led_scene"] == "danger")
  {
    scene_active = 1;
  }
  else if(doc["led_scene"] == "stop")
  {
    scene_active=0;
  }
      


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("led-menjador/in");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pixels.begin();
  pixels.clear();
}
byte send_alive = 0;

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 20) {
    send_alive++;
    if(send_alive == 100)
    {
      send_alive = 0;
      client.publish("led-menjador/out", "{'alive':1}");
    }
    lastMsg = now;
    if(scene_active == 1)
    {
       fade_in();
    }
    else
    {
       black();
    }
  }
}

void black()
{
  for(byte x =0; x< NUMPIXELS;x++)
  {
     pixels.setPixelColor(x, pixels.Color(0, 0, 0));
  }
  pixels.show();
}

byte timer_fade_in = 2;
void fade_in()
{
  static int pixel_print;
  static byte counter = 0;
  counter++;
  Serial.println(pixel_print);
  if(counter == timer_fade_in)
  {
      counter =0;
      pixel_print++;
      if(NUMPIXELS/2 < pixel_print)
      {
        pixel_print =0; 
      }
  }
  for(byte x =0; x< NUMPIXELS/2+1;x++)
  {
    if(x <= pixel_print)
    {
       pixels.setPixelColor(x, pixels.Color(0, 150, 0));
       pixels.setPixelColor(NUMPIXELS-x, pixels.Color(0, 150, 0));
    }
    else
    {
       pixels.setPixelColor(x, pixels.Color(0, 0, 0));
       pixels.setPixelColor(NUMPIXELS-x, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();
}
