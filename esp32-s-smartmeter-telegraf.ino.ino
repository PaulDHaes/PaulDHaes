#include <WiFi.h>
#include <WiFiUDP.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

const char* ssid = "test";
const char* password = "testing";
const char* udpAddress  = "ip";
const int udpPort = 8125; // Telegraf UDP port

WiFiUDP udp;

HardwareSerial smartMeterSerial(1); // Use the correct hardware serial and pins
struct ParsedData {
    String timestamp;
    int currentRate;
    float rate1TotalConsumption;
    float rate2TotalConsumption;
    float rate1TotalProduction;
    float rate2TotalProduction;
    float L1Consumption;
    float L2Consumption;
    float L3Consumption;
    float allPhasesConsumption;
    float L1Production;
    float L2Production;
    float L3Production;
    float allPhasesProduction;
    float L1Voltage;
    float L2Voltage;
    float L3Voltage;
    float L1Current;
    float L2Current;
    float L3Current;
    float waterConsumption;
    float gasConsumption;
};

ParsedData parsedData;

void setup() {
    Serial.begin(115200);
    const int RX_PIN = 16; // Example GPIO number for RX
    const int TX_PIN = 17; // Example GPIO number for TX
    smartMeterSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // Set correct pins

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    udp.begin(udpPort);
}

void loop() {
    if (smartMeterSerial.available()) {
        String data = readSmartMeterData();
        parseData(data);
        // Optionally, you can check if all necessary data is parsed
        // then send it to Telegraf
        sendDataToTelegraf();
    }
}

String readSmartMeterData() {
  String line = smartMeterSerial.readStringUntil('\n');
  return line;
}

void parseData(String line) {
if (line.startsWith("0-0:1.0.0")) { // Timestamp
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf(')'); // Adjust according to your timestamp format
  parsedData.timestamp = line.substring(startIndex, endIndex);
} else if (line.startsWith("0-0:96.14.0")) { // Current_rate_(1_is_day_and_2_is_night)
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf(')');
  parsedData.currentRate = line.substring(startIndex, endIndex).toInt();
} else if (line.startsWith("1-0:1.8.1")) { // Rate_1_(day)-total_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.rate1TotalConsumption = valueString.toFloat();
}else if (line.startsWith("1-0:1.8.2")) { // Rate_2_(night)-total_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.rate2TotalConsumption = valueString.toFloat();
} else if (line.startsWith("1-0:2.8.1")) { // Rate_1_(day)-total_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.rate1TotalProduction = valueString.toFloat();
} else if (line.startsWith("1-0:2.8.2")) { // Rate_2_(night)-total_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.rate2TotalProduction = valueString.toFloat();
} else if (line.startsWith("1-0:21.7.0")) { // L1_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L1Consumption = valueString.toFloat();
} else if (line.startsWith("1-0:41.7.0")) { // L2_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L2Consumption = valueString.toFloat();
} else if (line.startsWith("1-0:61.7.0")) { // L3_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L3Consumption = valueString.toFloat();
} else if (line.startsWith("1-0:1.7.0")) { // All_phases_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.allPhasesConsumption = valueString.toFloat();
} else if (line.startsWith("1-0:22.7.0")) { // L1_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L1Production = valueString.toFloat();
} else if (line.startsWith("1-0:42.7.0")) { // L2_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L2Production = valueString.toFloat();
} else if (line.startsWith("1-0:62.7.0")) { // L3_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L3Production = valueString.toFloat();
} else if (line.startsWith("1-0:2.7.0")) { // All_phases_production
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.allPhasesProduction = valueString.toFloat();
} else if (line.startsWith("1-0:32.7.0")) { // L1_voltage
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L1Voltage = valueString.toFloat();
} else if (line.startsWith("1-0:52.7.0")) { // L2_voltage
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L2Voltage = valueString.toFloat();
} else if (line.startsWith("1-0:72.7.0")) { // L3_voltage
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L3Voltage = valueString.toFloat();
} else if (line.startsWith("1-0:31.7.0")) { // L1_current
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L1Current = valueString.toFloat();
} else if (line.startsWith("1-0:51.7.0")) { // L2_current
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L2Current = valueString.toFloat();
} else if (line.startsWith("1-0:71.7.0")) { // L3_current
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.L3Current = valueString.toFloat();
} else if (line.startsWith("0-1:24.2.1")) { // water_consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.waterConsumption = valueString.toFloat();
} else if (line.startsWith("0-1:24.2.3")) { // Gas consumption
  int startIndex = line.indexOf('(') + 1;
  int endIndex = line.indexOf('*');
  String valueString = line.substring(startIndex, endIndex);
  parsedData.gasConsumption = valueString.toFloat();
}
}
void sendDataToTelegraf() {
    DynamicJsonDocument json(1024);
    // Populate the JSON object
    json["timestamp"] = parsedData.timestamp;
    json["currentRate"] = parsedData.currentRate;
    json["rate1TotalConsumption"] = parsedData.rate1TotalConsumption;
    json["rate2TotalConsumption"] = parsedData.rate2TotalConsumption;
    json["rate1TotalProduction"] = parsedData.rate1TotalProduction;
    json["rate2TotalProduction"] = parsedData.rate2TotalProduction;
    json["L1Consumption"] = parsedData.L1Consumption;
    json["L2Consumption"] = parsedData.L2Consumption;
    json["L3Consumption"] = parsedData.L3Consumption;
    json["allPhasesConsumption"] = parsedData.allPhasesConsumption;
    json["L1Production"] = parsedData.L1Production;
    json["L2Production"] = parsedData.L2Production;
    json["L3Production"] = parsedData.L3Production;
    json["allPhasesProduction"] = parsedData.allPhasesProduction;
    json["L1Voltage"] = parsedData.L1Voltage;
    json["L2Voltage"] = parsedData.L2Voltage;
    json["L3Voltage"] = parsedData.L3Voltage;
    json["L1Current"] = parsedData.L1Current;
    json["L2Current"] = parsedData.L2Current;
    json["L3Current"] = parsedData.L3Current;
    json["waterConsumption"] = parsedData.waterConsumption;
    json["gasConsumption"] = parsedData.gasConsumption;

    // Convert JSON object to String
    String jsonString;
    serializeJson(json, jsonString);

    // Send data over UDP
    udp.beginPacket(udpAddress, udpPort);
    udp.print(jsonString);
    udp.endPacket();
}