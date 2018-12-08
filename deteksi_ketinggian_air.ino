
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

const int trigPin = 3;
const int echoPin = 2;
const int buzPin = 4;
const int ketinggianWadah = 8;
int ketinggianAir;

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//publisher dan subscriber
IPAddress ip(10, 39, 52, 115);
IPAddress Mydns(10, 39, 52, 66);
IPAddress gateway(10, 39, 52, 254);
IPAddress subnet(255, 255, 255, 0);
//broker pengolah data
IPAddress server(10, 33, 109, 37);

// defines variables
long duration;
int distance;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(buzPin, OUTPUT);
  digitalWrite(buzPin, HIGH);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip, Mydns, gateway, subnet);
  // Allow the hardware to sort itself out
  delay(1000);

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("air")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("ketinggianAir");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
//
void sensorUltrasonic(int pinTrigger, int pinEcho)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

}


void loop() {
  sensorUltrasonic(trigPin, echoPin);
  ketinggianAir = ketinggianWadah - distance;
  if (ketinggianAir <= 0) {
    ketinggianAir = 0;
  }

  delay(1000);

  // Prints the distance on the Serial Monitor
  Serial.print("Ketinggian Air : ");
  Serial.print(ketinggianAir);
  Serial.println(" cm");
  digitalWrite(buzPin, LOW);
  if (ketinggianAir >= 6) {
    Serial.println(" [Tingkat Air] Awas");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(buzPin, LOW);
    delay(100);
    digitalWrite(buzPin, HIGH);
    delay(100);
  } else {
    digitalWrite(buzPin, HIGH);
  }

  //
  char buffer[10];
  dtostrf(ketinggianAir , 0, 0, buffer);
  if (!client.connected()) {
    reconnect();
  } else {
    client.publish("ketinggianAir", buffer);
  }
  client.loop();
}

