/* 
 *  To use this code, download the ESP32WebServer library from: https://github.com/Pedroalbuquerque/ESP32WebServer
 */

#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <SPI.h>
#include <SD.h>

const char* ssid = "*************";
const char* password = "*********";

ESP32WebServer server(80);  //Default port number

void handleRoot() {  
  /* we load the index.html from microSD */
  File myFile = SD.open("/index.html");
  if (myFile) {  
    /* respond the content of file to client by calling streamFile()*/
    size_t sent = server.streamFile(myFile, "text/html");
    /* close the file */
    myFile.close();
  } else {
    Serial.println("error opening index.html");
  }
}

//XML page to listen for motor commands
void handleMotors() { 
 String motorState = "OFF";
 String t_state = server.arg("motorState"); //Refer  xhttp.open("GET", "setMotors?motorState="+motorData, true);

 Serial.print("D\r\n"); //Disable motors
 delay(50);
 if(t_state.startsWith("U"))  //Drive Forward (UP Arrow)
 {
    if(t_state == "U1")
    {
      Serial.print("M0F70\r\n");
      delay(50);
      Serial.print("M1F68\r\n");
      delay(50);
      Serial.print("E\r\n");
    }
    else if(t_state == "U3")
    {
      Serial.print("M0F70\r\n");
      delay(50);
      Serial.print("M1F68\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(100);
      Serial.print("D\r\n");
    }
 }
 else if(t_state.startsWith("D")) //Reverse (DOWN Arrow)
 {
    if(t_state == "D1")
    {
      Serial.print("M0R70\r\n");
      delay(50);
      Serial.print("M1R68\r\n");
      delay(50);
      Serial.print("E\r\n");
    }
    else if(t_state == "D3")
    {
      Serial.print("M0R70\r\n");
      delay(50);
      Serial.print("M1R68\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(100);
      Serial.print("D\r\n");
    }
 }
 else if(t_state.startsWith("R")) //Turn Right (Right Arrow)
 {
    if(t_state == "R1")
    {
      Serial.print("M0R30\r\n");
      delay(50);
      Serial.print("M1F30\r\n");
      delay(50);
      Serial.print("E\r\n");
    }
    else if(t_state == "R2")
    {
      Serial.print("M0R50\r\n");
      delay(50);
      Serial.print("M1F50\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(200);
      Serial.print("D\r\n");
    }
    else if(t_state == "R3")
    {
      Serial.print("M0R50\r\n");
      delay(50);
      Serial.print("M1F50\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(100);
      Serial.print("D\r\n");
    }
 }
 else if(t_state.startsWith("L")) //Turn Left (LEFT Arrow)
 {
    if(t_state == "L1")
    {
      Serial.print("M0F30\r\n");
      delay(50);
      Serial.print("M1R30\r\n");
      delay(50);
      Serial.print("E\r\n");
    }
    else if(t_state == "L2")
    {
      Serial.print("M0F50\r\n");
      delay(50);
      Serial.print("M1R50\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(200);
      Serial.print("D\r\n");
    }
    else if(t_state == "L3")
    {
      Serial.print("M0F50\r\n");
      delay(50);
      Serial.print("M1R50\r\n");
      delay(50);
      Serial.print("E\r\n");
      delay(100);
      Serial.print("D\r\n");
    }
 }
 
 server.send(200, "text/plain", motorState); //Send web page
}
/* cannot handle request so return 404 */
void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /* register the callbacks to process client request */
  /* root request we will read the memory card to get 
  the content of index.html and respond that content to client */
  server.on("/", handleRoot);
  server.on("/setMotors", handleMotors);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
  Serial.print("Initializing SD card...");
  /* initialize microSD */
  if (!SD.begin(33)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop(void){
  server.handleClient();
}
