#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

//----------- Enter your Wi-Fi Details ---------//
char ssid[] = "WIFI_NAME"; // SSID
char pass[] = "WIFI_PASSWORD"; // Password
//----------------------------------------------//

const int trigger = D0;
const int echo = D1;
const int buzzer = D7;
long T;
float distanceCM;
WiFiClient client;

unsigned long myChannelField = 1111111; // Enter Channel ID
const int ChannelField = 1; // Which channel to write data
const char *myWriteAPIKey = "API Write Key"; // Your write API Key

int pushupTarget = 7;
int pushupCount = 0;
bool above30 = false;
bool below10 = false;

void setup()
{
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  Serial.println("yeahhhh");
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  digitalWrite(trigger, LOW);
  delayMicroseconds(1000);  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger, LOW);

  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034 / 2.0;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);

  if (distanceCM > 30 && !above30) {
    above30 = true;
  }
  if (distanceCM < 10 && above30 && !below10) {
    below10 = true;
  }
  if (distanceCM > 30 && below10) {
    above30 = false;
    below10 = false;
    if (pushupCount < pushupTarget) {
      pushupCount++;
      digitalWrite(buzzer, HIGH);
      delay(50);
      digitalWrite(buzzer, LOW);
      Serial.print("Push-up count: ");
      Serial.println(pushupCount);
    }
  }

  if (pushupCount >= pushupTarget)
  {
    playBuzzerTune();
  }

   ThingSpeak.writeField(myChannelField, ChannelField, distanceCM, myWriteAPIKey);
   delay(1000); // Delay between ThingSpeak updates

  Serial.print("Push-ups executed: ");
  Serial.println(pushupCount);
}

void playBuzzerTune()
{
  // Play a tune using the buzzer to indicate target completion
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
  delay(1000);
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  delay(2000);
}
