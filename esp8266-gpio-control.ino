#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

struct PinMapping {
  const String name;
  int value;
};

PinMapping pinMappings[] = {
  { "D0", 16 },
  { "D1", 5 },
  { "D2", 4 },
  { "D3", 0 },
  { "D4", 2 },
  { "D5", 14 },
  { "D6", 12 },
  { "D7", 13 },
  { "D8", 15 },
  { "D9", 3 },
  { "D10", 1 }
};

const char* apSSID = "NodeMCU";

ESP8266WebServer server(80);

char ssid[32];
char password[64];
IPAddress ip(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);

  if (loadCredentialsFromEEPROM()) {
    connectToWiFi();
  } else {
    createAP();
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/control", HTTP_GET, handleControl);
  server.begin();
}

void loop() {
  server.handleClient();
}

void createAP() {
  Serial.println("Setting AP...");
  WiFi.softAP(apSSID);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>WiFi config</h1>";
  html += "<form method='post' action='/save'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "IP: <input type='text' name='ip'><br>";
  html += "Subnet mask: <input type='text' name='subnet'><br>";
  html += "<input type='submit' value='Save'>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSave() {
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");
  String newIP = server.arg("ip");
  String newSubnet = server.arg("subnet");

  newSSID.toCharArray(ssid, 32);
  newPassword.toCharArray(password, 64);
  ip.fromString(newIP);
  subnet.fromString(newSubnet);

  saveCredentialsToEEPROM();
  connectToWiFi();

  server.send(200, "text/plain", "Configuration saved. Restarting NodeMCU...");
  delay(1000);
  ESP.restart();
}

bool loadCredentialsFromEEPROM() {
  EEPROM.get(0, ssid);
  EEPROM.get(32, password);
  EEPROM.get(96, ip);
  EEPROM.get(112, subnet);
  return (strlen(ssid) > 0);
}

void saveCredentialsToEEPROM() {
  EEPROM.put(0, ssid);
  EEPROM.put(32, password);
  EEPROM.put(96, ip);
  EEPROM.put(112, subnet);
  EEPROM.commit();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  WiFi.config(ip, ip, subnet);
  Serial.print("Connected to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nSuccessfull connection");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ---- GPIO handle ----

void handleControl() {
  if (server.args() > 0) {
    String pinStr = server.arg("pin");
    String action = server.arg("action");

    const int pin = getPinValue(pinStr);

    Serial.print("Pin value: ");
    Serial.println(pin);

    if (pin != 255) {
      if (action == "on") {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        String json_response = "{\"status\":\"Pin " + pinStr + " on\"}";
        server.send(200, "application/json", json_response);
      } else if (action == "off") {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        String json_response = "{\"status\":\"Pin " + pinStr + " off\"}";
        server.send(200, "application/json", json_response);
      } else {
        String json_response = "{\"error\":\"Invalid action\"}";
        server.send(400, "application/json", json_response);
      }
    } else {
      String json_response = "{\"error\":\"Invalid port number\"}";
      server.send(400, "application/json", json_response);
    }
  } else {
    String json_response = "{\"error\":\"Insufficient parameters\"}";
    server.send(400, "application/json", json_response);
  }
}

// Función para obtener el valor del pin a partir del nombre
int getPinValue(const String pinName) {
  for (int i = 0; i < sizeof(pinMappings) / sizeof(pinMappings[0]); i++) {
    if (pinName == pinMappings[i].name) {
      return pinMappings[i].value;
    }
  }
  return 255;
}
