/*

read values from sensor and publish using php
write values to pump using php
Led Color Codes 

Sensors used: LM35, soil moisture sensor, LDR
Pinout:
    Sensor/board  -> nodeMCU -> GPIO
    74151 1:8 MUX
               s0 -> d0 -> 16
               s1 -> d1 -> 5
               s2 -> d2 -> 4
               s3 -> d3 -> 0
              Sig -> A0 -> 17
    BatteryRelay
               in -> d7 -> 13
              out -> d8 -> 15
    pump
               1: -> d4 -> 2
               2: -> d5 -> 14
               3: -> d6 -> 12
    LED
               1: -> S2 -> 9
               2: -> S3 -> 10
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include "string.h"

//Variables for WiFi Connection
int statusCode;
const char* ssid = "nodeMCU ESP8266";
const char* passphrase = "";
String st;
String content;
WiFiClient client;

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

//Variables for Sensor Values
int s0=16;
int s1=5;
int s2=4;
int s3=0;
int inputMUX = A0;

int outputPump1 = 2;
int outputPump2 = 14;
int outputPump3 = 12;
int LEDR = 9;
int LEDB = 10;

int dbip[] = {0,0,0,0};

char printData[] = "Sensor : %d\tValue: %f\n";
char printD[300];

float super[13][2];

// Fuctions used for WiFi credentials saving and connecting to it which you do not need to change 
bool testWifi(void) {
    int c = 0;
    Serial.println("Waiting for Wifi to connect");
    while ( c < 20 ) {
        //set led-blue to high till wifi connects
        digitalWrite(LEDR,LOW);
        digitalWrite(LEDB,HIGH);

        if (WiFi.status() == WL_CONNECTED) {
            //set led-blue to low when connected
            digitalWrite(LEDR,LOW);
            digitalWrite(LEDB,LOW);

            return true;
        }
    delay(500);
    Serial.print("*");
    c++;
    }
    //set led-red high when wifi failed to connect
    digitalWrite(LEDB,LOW);
    digitalWrite(LEDR,HIGH);

    Serial.println("");
    Serial.println("Connect timed out, opening AP");
    return false;
}

void launchWeb() {
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("WiFi connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer();
    // Start the server
    server.begin();
    Serial.println("Server started");
}

void setupAP(void) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
        Serial.println("no networks found");
    else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    WiFi.softAP("nodeMCU_ESP8266", "qwertyasdf");
    Serial.println("softap");
    launchWeb();
    Serial.println("over");
}

void clearEEPROM(){
    char c;
    Serial.println("Resetting ROM");
    Serial.println(EEPROM.length());
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        c = (char)EEPROM.read(i);
        Serial.println(c);
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    //set the built in led low(on) when we're done for 5 seconds
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
    digitalWrite(LED_BUILTIN, HIGH);
}

void createWebServer() {
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 ";
        content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><br><label>Password: </label><input name='pass' length=64><br><label>Server Ip:</label><input name='sip' length=15><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);
    });

    server.on("/scan", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>go back";
        server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        String serverip = server.arg("sip");
        if (qsid.length() > 0 && qpass.length() > 0) {
            Serial.println("clearing eeprom");
            for (int i = 0; i < 96; ++i) {
                EEPROM.write(i, 0);
            }
            Serial.println(qsid);
            Serial.println("");
            Serial.println(qpass);
            Serial.println("");

            Serial.println("writing eeprom ssid:");
            for (int i = 0; i < qsid.length(); ++i) {
                EEPROM.write(i, qsid[i]);
                Serial.print("Wrote: ");
                Serial.println(qsid[i]);
            }
            Serial.println("writing eeprom pass:");
            for (int i = 0; i < qpass.length(); ++i) {
                EEPROM.write(32 + i, qpass[i]);
                Serial.print("Wrote: ");
                Serial.println(qpass[i]);
            }
            Serial.println("writing server IP:");
            for(int i = 0; i < serverip.length(); ++i){
                EEPROM.write(50 + i, serverip[i]);
                Serial.print("Wrote:");
                Serial.println(serverip[i]);
            }
            EEPROM.commit();

            content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
            statusCode = 200;
            ESP.reset();
        } 
        else {
            content = "{\"Error\":\"404 not found\"}";
            statusCode = 404;
            Serial.println("Sending 404");
        }
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", content);

    });
  }

//Functions for reading sensor inputs
void read() {
    float temp[3];
    int moist[6];
    int ldr[4];
    int sensorValue;
    float temporary = (3.3/1024)*100;
    int sid1 = 1001;
    int sid2 = 2001;
    int sid3 = 3001;

    for (int i = 1; i < 7; i++) {
        moist[i-1] = readMUX(i);
        super[i-1][0] = (float)moist[i-1];
        super[i-1][1] = sid1++;
    }
    for (int i = 8; i < 12; i++) {
        ldr[i-8] = readMUX(i);
        super[i-2][0] = (float)ldr[i-8];
        super[i-2][1] = sid2++;
    }
    for (int i = 13; i < 16; i++) {
        sensorValue = readMUX(i);
        sensorValue *= temporary;
        temp[i-13] = sensorValue;
        super[i-3][0] = (float)temp[i-13];
        super[i-3][1] = sid3++;
    }
}

float readMUX(int channel) {
    int controlPin[] = {s0,s1,s2,s3};
    int muxChannel[16][4] {
        {0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},
        {0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},
        {0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},
        {0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}
    };
    for (int i = 0; i < 4; i++) {
        digitalWrite(controlPin[i],muxChannel[channel][i]);
    }
    return analogRead(inputMUX);
}

//Function for Displaying Values
void display() {
    for(int i=0;i<13;i++) {
        sprintf(printD,printData,(int)super[i][1],super[i][0]);
        Serial.println(printD);
    }
}

//Function to publish values to MySQL
char serverName[100];
String apiKeyValue = "1qaz2wsx3edc";
char requestData[] = "apikey%s &SensorID%f &Sensorvalue%f";

void publish() {
    int failCounter = 0;
    for (int i = 0; i < 13; i++) {
        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            String httpRequestData = "apikey=" + apiKeyValue + "&SensorID=" 
            + (int)super[i][1] + "&SensorValue=" + (int)super[i][0];
            Serial.print("http request data:");
            Serial.println(httpRequestData);
            int httpResponseCode = http.POST(httpRequestData);
            if (httpResponseCode > 0) {
                Serial.print("Response Code:");
                Serial.println(httpResponseCode);
            }
            else if (failCounter > 5) {
                Serial.println("Server Connection Failed too many times.\nClearing EEPROM and Rebooting");
                ESP.reset();
            }
            else if (httpResponseCode == -1) {
                Serial.print("Error Code:");
                Serial.println(httpResponseCode);
                Serial.println("Server Connection Failed");
                ++failCounter;
            }
            else {
                Serial.print("Error Code:");
                Serial.println(httpResponseCode);
            }
        http.end();
    }
}

char linkName[200];

int payLoad (int action_id) {
    HTTPClient http;  
    String getData;
    String Link;
    getData = "?action_id=";
    Link = linkName + getData + action_id;
    Serial.println(getData);
    Serial.println(Link);
    http.begin(Link);
    int httpCode = http.GET();
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.print("ActionID=");
    Serial.print(action_id);
    Serial.print(" value=");
    Serial.println(payload);
    int returnValue = payload.toInt();
    http.end();
    return returnValue; 
}

int sid[] = {6001,6002,6003};
int val[3];

void callPayload() {
    for (int i = 0; i< 3; i++) {
            val[i] = payLoad(sid[i]);
            Serial.println(val[i]);
        }
    if (val[0] == 0) {
        Serial.println("switching pump1 OFF");
        digitalWrite(outputPump1,LOW);
        delay(100);
    }
    else if (val[0] == 1) {
        Serial.println("switching pump1 ON");
        digitalWrite(outputPump2,HIGH);
        delay(100);
    }
    if (val[1] == 0) {
        Serial.println("switching pump2 OFF");
        digitalWrite(outputPump2,LOW);
        delay(100);
    }
    else if (val[1] == 1) {
        Serial.println("switching pump2 ON");
        digitalWrite(outputPump2,HIGH);
        delay(100);
    }
    if (val[2] == 0) {
        Serial.println("switching pump3 OFF");
        digitalWrite(outputPump2,LOW);
        delay(100);
    }
    else if (val[2] == 1) {
        Serial.println("switching pump3 ON");
        digitalWrite(outputPump3,HIGH);
        delay(100);
    }
    else { }
}

void setup() {
    //set both led to initiate starting of the program for 3 seconds
    digitalWrite(LEDR,HIGH);
    digitalWrite(LEDB,HIGH);
    delay(3000);
    digitalWrite(LEDR,LOW);
    digitalWrite(LEDB,LOW);

    //setup pins
    pinMode(s0,OUTPUT);
    pinMode(s1,OUTPUT);
    pinMode(s2,OUTPUT);
    pinMode(s3,OUTPUT);
    pinMode(outputPump1,OUTPUT);
    pinMode(outputPump2,OUTPUT);
    pinMode(outputPump3,OUTPUT);
    
    Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
    Serial.println();
    WiFi.disconnect();
    EEPROM.begin(150); //Initialasing EEPROM
    delay(10);
    Serial.println("Startup");

    //Read eeprom for ssid, pass
    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i) {
        esid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.println("Reading EEPROM pass");
    String epass = "";
    for (int i = 32; i < 50; ++i) {
        epass += char(EEPROM.read(i));
    }
    Serial.print("PASS: ");
    Serial.println(epass);
    char ch;
    String IPS;
    for (int i = 50; i< 70; ++i){
        ch = char(EEPROM.read(i));
        if(ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0' || ch == '.') {
            IPS += ch;
        }
        else{
            continue;
        }
    }
    int n = IPS.length() + 1;
    char CIPS[n];
    strcpy(CIPS,IPS.c_str());
    sscanf(CIPS,"%d.%d.%d.%d", &dbip[0],&dbip[1],&dbip[2],&dbip[3]);
    Serial.print("Server IP:");
    sprintf(serverName, "http://%s/postEspData.php", CIPS);
    sprintf(linkName, "http://%s/getStatus.php", CIPS);
    for(int i = 0; i < 4; i++){
        Serial.print(dbip[i]);
        Serial.print(".");
    }
    Serial.println("");
    WiFi.begin(esid.c_str(), epass.c_str());
    if (testWifi()) {
        //set led-blue flickering 5 times to initiate success of the connecting wifi
        digitalWrite(LEDR,LOW);
        digitalWrite(LEDB,HIGH);
        delay(500);
        digitalWrite(LEDB,LOW);
        delay(500);
        digitalWrite(LEDB,HIGH);
        delay(500);
        digitalWrite(LEDB,LOW);
        delay(500);
        digitalWrite(LEDB,HIGH);
        delay(500);
        digitalWrite(LEDB,LOW);
        delay(500);
        digitalWrite(LEDB,HIGH);
        delay(500);
        digitalWrite(LEDB,LOW);
        delay(500);
        digitalWrite(LEDB,HIGH);
        delay(500);
        digitalWrite(LEDB,LOW);

        Serial.println("Succesfully Connected!!!");
        Serial.println(WiFi.localIP());
        return;
    }
    else {
        //set led-red flickering 3 times to initiate failure of the connecting wifi
        digitalWrite(LEDB,LOW);
        digitalWrite(LEDR,HIGH);
        delay(500);
        digitalWrite(LEDR,LOW);
        delay(500);
        digitalWrite(LEDR,HIGH);
        delay(500);
        digitalWrite(LEDR,LOW);
        delay(500);
        digitalWrite(LEDR,HIGH);
        delay(500);
        digitalWrite(LEDR,LOW);

        Serial.println("Turning the HotSpot On");
        clearEEPROM();
        launchWeb();
        setupAP();
    }
    Serial.println();
    Serial.println("Waiting.");
    while ((WiFi.status() != WL_CONNECTED)) {
        Serial.print(".");
        delay(100);
        server.handleClient();
    }
}

//main loop
void loop() {
    if ((WiFi.status() == WL_CONNECTED)) {
        read();
        delay(1000);
        display();
        delay(1000);
        publish();
        delay(1000);
        callPayload();
        delay(10000);
        
    }
    else {
        Serial.println("WiFi disconnected");
    }
}
