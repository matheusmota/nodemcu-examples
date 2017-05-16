//This example will set up a static IP - in this case 192.168.1.50
  
#include <ESP8266WiFi.h>
 
const int   relayPin = D1;
const int   chipSelect = D8;
const char* ssid = "MYSSID";
const char* password = "SECRET";
  
WiFiServer server(8080);
IPAddress ip(192, 168, 1, 50); // where xx is the desired IP Address
IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
   
void setup() {

  Serial.begin(115200);
  delay(10);
  
  pinMode(relayPin, OUTPUT);
  
  //Serial.print(F("Setting static ip to : "));
  //Serial.println(ip);
  //WiFi.config(ip, gateway, subnet); 
  
  // Connect to WiFi network
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
  
void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // Match the request
  
  int value = LOW;

  if (request.indexOf("/relay=on") != -1) {
    digitalWrite(relayPin, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/relay=off") != -1){
    digitalWrite(relayPin, LOW);
    value = LOW;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.print("Led pin is now: ");
  
  if(value == HIGH) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/relay=on\">Relay ON</a><br>");
  client.println("Click <a href=\"/relay=off\">Relay OFF</a>");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
