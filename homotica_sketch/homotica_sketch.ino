//Sketch for Homotica - Author Davide Vertuani
#include <Homotica.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EthernetUdp.h>

//#define DEBUG_MODE                                       //uncomment this line if you want serial outputs
#define SKETCH_NAME "HomoticaSketch"
#define SKETCH_VERSION "1.0"
#define ACTIVE_LOW                                         //uncomment this line if you're using active_high setup

#ifdef ACTIVE_LOW
#define MHIGH 0x0
#define MLOW  0x1
#else
#define MHIGH 0x1
#define MLOW  0x0
#endif

#ifdef DEBUG_MODE
#define DEBUG_BEGIN(x) Serial.begin(x)
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_BEGIN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

//MODIFY THESE PARAMETERS
IPAddress ip(192, 168, 1, 20);                          //modify here your arduino IP
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      //modify here your arduino MAC address
unsigned int localPort = 80;                            //modify here your arduino port
static String code = "xxxxxxxx";                        //modify here your 8-digits auth code
static int usedPinRange[] = {1,13};                     //the pins used as output

char packetBuffer[300];
char positiveResponse[] = "1";
char negativeResponse[] = "0";
EthernetUDP Udp;
Homotica homotica;

void setup() {
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  SPI.begin();

  DEBUG_BEGIN(9600);
  DEBUG_PRINT("UDP is at ");
  DEBUG_PRINTLN(Ethernet.localIP());
  DEBUG_PRINTLN("");

  for (int i = usedPinRange[0]; i < usedPinRange[1] + 1;  i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, MLOW);
  }
}

void loop() {
  checkUDP();
  homotica.refresh();
}

void checkUDP() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {

    DEBUG_PRINTLN();
    DEBUG_PRINTLN("********************NEW INCOMING PACKET**************************");

    Udp.read(packetBuffer, sizeof(packetBuffer));
    String msg = packetBuffer;
    int andIndex = msg.indexOf("?");
    int eqIndex = msg.indexOf("=");
    String recivedCode = msg.substring(eqIndex + 1 , msg.indexOf("!"));
    String request = msg.substring(andIndex + 1 , eqIndex);
    if (recivedCode == code) {
      String mSubstring = msg.substring(msg.indexOf("!"));

      DEBUG_PRINT("Recived: ");
      DEBUG_PRINTLN(msg);
      DEBUG_PRINT("Substring: ");
      DEBUG_PRINTLN(mSubstring);

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      if (request == "QU") {
        int PIN = mSubstring.substring(mSubstring.indexOf("!")).toInt();
        String sensorReading = String(analogRead(PIN));
        char output[sensorReading.length() + 1];
        sensorReading.toCharArray(output, sensorReading.length() + 1);
        Udp.write(output);
      }
      else if (request == "ST") {
        Udp.write(positiveResponse);
        Udp.endPacket();
        processSingleRunnable(mSubstring);
      }
      else if (request == "MT") {
        Udp.write(positiveResponse);
        Udp.endPacket();
        processMultipleRunnable(mSubstring);
      }
      else if (request == "CH") {
        Udp.write(positiveResponse);
        Udp.endPacket();
      }
      else {
        Udp.write(negativeResponse);
        Udp.endPacket();
      }
    }
    for ( int i = 0; i < sizeof(packetBuffer);  ++i ) {
      packetBuffer[i] = (char)0;
    }
  }
  delay(10);
}

void processSingleRunnable(String msg) {
  boolean accepted = false;
  int charsIndex[4];
  charsIndex[0] = msg.indexOf("!");
  charsIndex[1] = msg.indexOf("!", charsIndex[0] + 1);
  charsIndex[2] = msg.indexOf("!", charsIndex[1] + 1);
  charsIndex[3] = msg.indexOf("!", charsIndex[2] + 1);
  int PIN = msg.substring(charsIndex[0] + 1 , charsIndex[1]).toInt();
  int relayMode = msg.substring(charsIndex[1] + 1, charsIndex[2]).toInt();
  int del = msg.substring(charsIndex[2] + 1, charsIndex[3]).toInt();

  DEBUG_PRINTLN();
  DEBUG_PRINTLN("- OPENING -");
  DEBUG_PRINT("Pin: ");
  DEBUG_PRINT(String(PIN));
  DEBUG_PRINT("; Mode: ");
  DEBUG_PRINT(String(relayMode));
  DEBUG_PRINT("; Delay: ");
  DEBUG_PRINTLN(msg.substring(charsIndex[2] + 1, charsIndex[3]));
  DEBUG_PRINTLN("(0 --> ON, 1 --> OFF, 2 --> PUSH)");
  DEBUG_PRINTLN();

  if (0 <= relayMode <= 2 && usedPinRange[0] <= PIN <= usedPinRange[1])
  {
    if (relayMode == 0) {
      digitalWrite(PIN, MHIGH); //relay on
    }
    else if (relayMode == 1) {
      digitalWrite(PIN, MLOW);
    }
    else if (relayMode == 2) {
      homotica.pushPin(PIN, del, digitalRead(PIN));
    }
    else if (relayMode == 3) {
      digitalWrite(PIN, !digitalRead(PIN));
    }
  }
}

void processMultipleRunnable(String msg) {
  char input[msg.length() + 1];
  msg.toCharArray(input, msg.length() + 1);
  const char* divider = ("+");
  int lastIndex = 0;
  for (int k = 0; k < sizeof(input); k++) {
    if (String(input[k]) == "+") {
      processSingleRunnable(msg.substring(lastIndex, k));
      k += 1;
      lastIndex = k;
    }
  }
}
