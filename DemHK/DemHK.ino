// Load Wi-Fi library
#include <ESP8266WiFi.h>
// Replace with your network credentials
const char* ssid     = "NguyenThanhLoi";
const char* password = "1653020006";

WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
// Assign output variables to GPIO pins
#define sensorIN D2
#define sensorOUT D1
// Current time
unsigned long currentTime = millis();
// Previous time
int count = 0;
unsigned int ava = 0 ;
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
void setup() {
  Serial.begin(115200);
  pinMode(sensorIN, INPUT);
  pinMode(sensorOUT, INPUT);
  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
//  delay(1000);
// Connect to Wi-Fi network with SSID and password
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  // Print local IP address and start web server
//  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
  server.begin();
}
void loop() {
  ////////////////////////Main//////////////////////////////////////

  int var = digitalRead(sensorIN);
  int out = digitalRead(sensorOUT);
  if (var == LOW)
  {
    count += 1;
    delay(500);
    ava += 1;
  }
  if (out == LOW)
  {
    count -= 1;
    delay(500);
  }



  //////////////////////////////WEB SITE/////////////////////////////
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta  charset=\"UTF-8\" http-equiv='refresh' content='0.5'\name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>Nguyễn Thành Lợi</title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body style=\"background-image: url('https://d3av3o1z276gfa.cloudfront.net/images/place/gYsaVvlH265n2JW3YW9UNYNIm0qDwuK8.jpeg');background-repeat: no-repeat;background-attachment: fixed;background-position: bottom;background-size: 300px 400px;height: 1600;width:2560;\"><h1 style='font-size: 60px;'>Number airport passenger </h1>");
            client.print("<strong><p style='font-size: 35px;text-align: left;margin-left: 450px;color: red;'>Total : &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a>");
            client.print(ava);
            client.print("</a></p></strong>");
            client.print("<strong><p style='font-size: 35px;text-align: left;margin-left: 450px;color: green;'>Available : &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a>");
            if(count < 0)
            {
              client.print("0");
              }
              else{
                client.print(count);
                }
            
            client.println("</a></p></strong>");
            client.println("<body style=\"background-image: url('https://d3av3o1z276gfa.cloudfront.net/images/place/gYsaVvlH265n2JW3YW9UNYNIm0qDwuK8.jpeg');background-repeat: no-repeat;background-attachment: fixed;background-position: bottom;background-size: 300px 400px;height: 1600;width:2560;\"><h1 style='font-size: 50px;'>Information: </h1>");
            client.print("<strong><p style='font-size: 35px;text-align: left;margin-left: 450px;'>Name : Nguyễn Thành Lợi <a>");
            client.print("<strong><p style='font-size: 35px;text-align: left;margin-left: 450px;'>MSSV : 1653020006 <a>");
            
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
