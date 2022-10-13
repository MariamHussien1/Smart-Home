#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<Servo.h>

//wifi decleration 
const char* ssid = "Mariam";
const char* password = "Mariam1062";
const char* host ="api.thingspeak.com";
const char* ssid1 = "ESP8266";
const char* password1 = "12345678";

Servo door;
ESP8266WebServer server(80);

uint8_t LED1pin = D1;
bool LED1status = LOW;
uint8_t LED2pin = D2;
bool LED2status = LOW;
uint8_t LED3pin = D3;
bool LED3status = LOW;
uint8_t LED4pin = D5;
bool LED4status = LOW;
void setup()
{ 
  door.attach(2);
  door.write(0);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
  pinMode(LED4pin, OUTPUT);
 
  Serial.begin(115200);
  Serial.println();
  
  //connecting to wifi
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  
  //connecting to nodemcu
  WiFi.softAP(ssid1, password1);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.println(WiFi.localIP());
  delay(100);

  //led1
  server.on("/", handle_OnConnect1);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  //led2
  server.on("/", handle_OnConnect2);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  //led3
  server.on("/", handle_OnConnect3);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  //led4
  server.on("/", handle_OnConnect4);
  server.on("/led4on", handle_led4on);
  server.on("/led4off", handle_led4off);
  
  server.begin();
  Serial.println("HTTP server started");
}


void loop()
{
  
  WiFiClient client;

  String line;
  //leds
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);
  Serial.print("Led 1 is on");}
  else
  {digitalWrite(LED1pin, LOW);
  Serial.print("Led 1 is off");}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);
  Serial.print("Led 2 is on");}
  else
  {digitalWrite(LED2pin, LOW);
  Serial.print("Led 2 is off");}
  
  if(LED3status)
  {digitalWrite(LED3pin, HIGH);
  Serial.print("Led 3 is on");}
  else
  {digitalWrite(LED3pin, LOW);
  Serial.print("Led 3 is off");}

  if(LED4status)
  {digitalWrite(LED4pin, HIGH);
  Serial.print("Led 4 is on");}
  else
  {digitalWrite(LED4pin, LOW);
  Serial.print("Led 4 is off");}

  //door
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET /") + "/channels/1492079/feeds.json?results=2" +
                 "HTTP/1.1\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        line = client.readString();
        Serial.println(line);
        int x = line.length();
        String y = line.substring(x - 5);
       
        if (y[0] == '1'){
          door.write(180);
          Serial.println("Door is open");
        }
        else if (y[0] == '0')
        {
          door.write(0);
          Serial.println("Door is closed");
        }
      }
    }

    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
}
//led1functions
void handle_OnConnect1() {
  LED1status = HIGH;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,LED4status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,LED3status,LED4status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,LED3status,LED4status)); 
}

//led2functions
void handle_OnConnect2() {
  LED2status = HIGH;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,LED4status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,LED3status,LED4status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,LED3status,LED4status)); 
}

//led3functions
void handle_OnConnect3() {
  LED3status = HIGH;
  Serial.println("GPIO0 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,LED4status)); 
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO0 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,true,LED4status)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO0 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,false,LED4status)); 
}

//led4functions
void handle_OnConnect4() {
  LED4status = HIGH;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,LED4status)); 
}

void handle_led4on() {
  LED4status = HIGH;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,true)); 
}

void handle_led4off() {
  LED4status = LOW;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,false)); 
}

//HTML
String SendHTML(uint8_t led1stat,uint8_t led2stat,uint8_t led3stat,uint8_t led4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h2 {color: #444444;margin: 50px auto 30px;}";
  ptr +=".button {display: block;width: 80px;background-color: Black;border: none;color: White;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on1 {background-color: #ADDFFF;}\n";
  ptr +=".button-on1:active {background-color: #A0CFEC;}\n";
  ptr +=".button-off1 {background-color: #616D7E;}\n";
  ptr +=".button-off1:active {background-color: #5E7D7E;}\n";

  ptr +=".button-on2 {background-color: #ADDFFF;}\n";
  ptr +=".button-on2:active {background-color: #A0CFEC;}\n";
  ptr +=".button-off2 {background-color: #616D7E;}\n";
  ptr +=".button-off2:active {background-color: #5E7D7E;}\n";

  ptr +=".button-on3 {background-color: #ADDFFF;}\n";
  ptr +=".button-on3:active {background-color: #A0CFEC;}\n";
  ptr +=".button-off3 {background-color: #616D7E;}\n";
  ptr +=".button-off3:active {background-color: #5E7D7E;}\n";

  ptr +=".button-on4 {background-color: #ADDFFF;}\n";
  ptr +=".button-on4:active {background-color: #A0CFEC;}\n";
  ptr +=".button-off4 {background-color: #616D7E;}\n";
  ptr +=".button-off4:active {background-color: #5E7D7E;}\n";
  
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h2>LEDs control</h2>\n";
  
   if(led1stat)
  {ptr +="<p>LED in room1: ON</p><a class=\"button button-off1\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED in room1: OFF</p><a class=\"button button-on1\" href=\"/led1on\">ON</a>\n";}
  if(led2stat)
  {ptr +="<p>LED in room2: ON</p><a class=\"button button-off2\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED in room2: OFF</p><a class=\"button button-on2\" href=\"/led2on\">ON</a>\n";}
  if(led3stat)
  {ptr +="<p>LED in room3: ON</p><a class=\"button button-off3\" href=\"/led3off\">OFF</a>\n";}
  else
  {ptr +="<p>LED in room3: OFF</p><a class=\"button button-on3\" href=\"/led3on\">ON</a>\n";}
  if(led4stat)
  {ptr +="<p>LED in room4: ON</p><a class=\"button button-off4\" href=\"/led4off\">OFF</a>\n";}
  else
  {ptr +="<p>LED in room4: OFF</p><a class=\"button button-on4\" href=\"/led4on\">ON</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}
