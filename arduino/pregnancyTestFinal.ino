// To Print Data setup
String dataFromServer;
String result;
int timePassed;

//OLED setup ------------------------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example


//WIFI setup ----------------------------
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "itpsandbox";
const char* password = "NYU+s0a!+P?";

const String url = "http://128.122.6.250:5000/result";
String payload;

// Allocate JsonBuffer
// Use arduinojson.org/assistant to compute the capacity.
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(10) + 260;
DynamicJsonBuffer jsonBuffer(capacity);

JsonObject& getData() {
  String message = "";
  
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;
    http.begin(url);         //Specify the URL
    int httpCode = http.GET();          //Make the request

    if (httpCode > 0) {                 //Check for the returning code
      payload = http.getString();
      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(payload);
      //Serial.println(payload);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        //        return;
      }
      return root;
    }
    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }
}

void setup() {
  Serial.begin(115200);
  
  //Get DATA ---------------------------------------------
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // Get and parse data
   dataFromServer = getData()["data"][0]["details"]["model_inputs"]["age"].as<char*>();

   Serial.println("Your age is: ");
   Serial.println(getData()["data"][0]["details"]["model_inputs"]["age"].as<char*>());
   Serial.println("You should not have a baby");
    
  //disconnect WiFi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  //Display DATA ---------------------------------------------
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer content on the screen --
  // the library initializes this with an Adafruit splash screen.
//  display.display();
//  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loading(void) {
  disappear();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.println("Loading");
  display.setCursor(20, 17);
  display.println("DNA.");
  display.display(); 
  delay(1000);

  disappear();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.println("Loading");
  display.setCursor(20, 17);
  display.println("DNA..");
  display.display(); 
  delay(1000);

  disappear();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.println("Loading");
  display.setCursor(20, 17);
  display.println("DNA...");
  display.display(); 
  delay(1000);


}


void showResult(void) {
  display.clearDisplay();
  delay(500);
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(50, 0);
  display.println("age:");
  display.setCursor(50, 17);
  display.println(dataFromServer);
  display.display(); 
  delay(250);
}

void disappear(void){
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(".");
  display.display(); 
  delay(800);
  }

void pregnant(void) {
  display.clearDisplay();
  for(int16_t i=0; i<display.height(); i+=1) {
    // The INVERSE color is used so rectangles alternate white/black
    display.drawLine(73, 0, 73, i, INVERSE);
    display.drawLine(74, 0, 74, i, INVERSE);
    display.drawLine(75, 0, 75, i, INVERSE);
    
    display.drawLine(99, 0, 99, i, INVERSE);
    display.drawLine(100, 0, 100, i, INVERSE);
    display.drawLine(101, 0, 101, i, INVERSE);

  }
  display.display();
  delay(2000);
}

void notPregnant(void) {
  display.clearDisplay();
  for(int16_t i=0; i<display.height(); i+=1) {
    // The INVERSE color is used so rectangles alternate white/black
    display.drawLine(73, 0, 73, i, INVERSE);
    display.drawLine(74, 0, 74, i, INVERSE);
    display.drawLine(75, 0, 75, i, INVERSE);
  }
  display.display();
  delay(2000);
}

void loop() {
  timePassed = millis();
  if (timePassed < 9000){
    loading();
  } else{
    showResult();
  }
}
