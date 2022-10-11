/*
 *  This sketch sends data via HTTP GET requests to $host.
 *
 *  Magic numbers are used to determine which line(s) to handle, and which part of this line is used.
 *  The numbers are determined using curl (or some other http dump program)
 */

#include <ESP8266WiFi.h>

const char* ssid     = "JustUseYourPhone";
const char* password = "PhonesHaveWifi";
//NEW URL: https://teamtrees.org/
const char* host = "teamtrees.org";
// This is the path to the page we want on the server:
String url = "/";

String theStringWeAreLookingFor = "easyScreenShotCounter"; //the total number of trees is in this line.
  

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  int linecounter=0;
  String vindchillString;
  float vindchill;
  String line="";
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  client.setInsecure();
  const int httpPort = 443;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  // The header should look like this:
  /*
> GET /data/stenomuseet.htm HTTP/1.1
> Host: vejret.stenomuseet.dk
> Connection: close
>  
  
  */
  client.print(
              String("GET ") + url + " HTTP/1.1\r\n" +  //Server + protocol
               "Host: " + host + "\r\n" + // Host
               "Connection: close\r\n" +  // I don't want to talk after you sent me the data.
               "\r\n"                     // Blank line at end of header.
              );
  
  delay(10); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  while(client.available()){
    line = client.readStringUntil('\r'); //cariage return as delimiter
    //Serial.println(line); //print out every received line.
    
    int result = line.indexOf(theStringWeAreLookingFor);
    if(result != -1) {
      Serial.print("Found it! its here! -> "); 
      Serial.println(line);
      
        //// THIS IS WHERE I would find the correct part of the String and parse it to a number i could use for something nice.
      // something like the .subString() and .parseInt() methods of the String should do it.
  

    }
    
    yield(); //Avoid a crash by letting the ESP core handle its wifi stuff.
  }

    
    
  
  Serial.println();
  Serial.println("closing connection");
  
  delay(5*60*1000); //run every 5 minutes (in milliseconds)

}