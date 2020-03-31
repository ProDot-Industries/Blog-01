/*
Sensors used: LM35, soil moisture sensor, LDR
Pinout:
    Sensor/board  -> nodeMCU -> GPIO
    HCSR04-1T -> D0 -> 16
    HCSR04-1E -> D1 -> 5
    HCSR04-2T -> D2 -> 4
    HCSR04-2E -> D3 -> 0
    HCSR04-3T -> D4 -> 2
    HCSR04-3E -> D5 -> 14
    HCSR04-4T -> D6 -> 12
    HCSR04-4E -> D7 -> 13
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <string.h>

//Pin setup
int HCST[] = {16,4,2,12};
int HCSE[] = {5,0,14,13};
int dbip[] = {0,0,0,0};
//Variables for WiFi Connection
int statusCode;
const char* ssid = "nodeMCU ESP8266 Security";
const char* passphrase = "";
String st;
String content;
WiFiClient client;



//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

//Variables for Sensor Values

// Fuctions used for WiFi credentials saving and connecting to it which you do not need to change
bool testWifi(void) {
    int c = 0;
    Serial.println("Waiting for Wifi to connect");
    while ( c < 30 ) {
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
    delay(500);
    Serial.print("*");
    c++;
    }
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
    WiFi.softAP("ESP_Security", "qwertyasdf");
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
    // turn the LED on when we're done
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}

void createWebServer() {
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><br><label>Password: </label><input name='pass' length=64><br><label>Server Ip:</label><input name='sip' length=15><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);
    });

    server.on("/scan", []() {
        //setupAP();
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
int super[4][2];

void read() {
    int id = 4001;
    for (int i = 0; i < 4; i++) {
        super[i][0] = getDistance(HCST[i],HCSE[i]);
        super[i][1] = id++;
    }
}

//function to get distance
int getDistance(int HCT, int HCE) {
    digitalWrite(HCT, LOW);
    delayMicroseconds(2);
    digitalWrite(HCT, HIGH);
    delayMicroseconds(10);
    digitalWrite(HCT, LOW);
    long int duration = pulseIn(HCE, HIGH);
    float distance = duration*0.034/2;
    return (int)distance;
}

//Function for Displaying Values
void display() {
    for (int i = 0; i < 4; i++) {
        Serial.print(super[i][1]);
        Serial.print("-");
        Serial.println(super[i][0]);
    }
}

//Function to publish values to MySQL
char serverName[500];
String apiKeyValue = "1qaz2wsx3edc";
char requestData[] = "apikey%s &SensorID%f &Sensorvalue%f";


void publish() {

    Serial.println("inside publish function");
    
    for (int i = 0; i < 4; i++) {
        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            String httpRequestData = "apikey=" + apiKeyValue + "&SensorID=" + super[i][1] + "&SensorValue=" + super[i][0];
            Serial.print("http request data:");
            Serial.println(httpRequestData);
            int httpResponseCode = http.POST(httpRequestData);
            if (httpResponseCode > 0) {
                Serial.print("Response Code:");
                Serial.println(httpResponseCode);
            }
            else {
                Serial.print("Error Code:");
                Serial.println(httpResponseCode);
            }
        http.end();
    }
}

void setup() {
    for (int i = 0; i < 4; i++){
        pinMode(HCSE[i],INPUT);
        pinMode(HCST[i],OUTPUT);
    }
    
    Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
    Serial.println("");
    WiFi.disconnect();
    EEPROM.begin(512); //Initialasing EEPROM
    delay(10);
    Serial.println("Startup");

    //Read eeprom for ssid, pass, server ip
  
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

    WiFi.begin(esid.c_str(), epass.c_str());
    if (testWifi()) {
        Serial.println("Succesfully Connected!!!");
        Serial.println(WiFi.localIP());
        return;
    }
    else {
        Serial.println("Turning the HotSpot On");
        launchWeb();
        setupAP();// Setup HotSpot
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
        delay(10000);
    }
    else {
        Serial.println("WiFi disconnected");
    }
}