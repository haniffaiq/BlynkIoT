#define BLYNK_TEMPLATE_ID "TMPLjXwjEeXW"
#define BLYNK_DEVICE_NAME "Tugas Akhir"
#define BLYNK_AUTH_TOKEN "rsHReQUIa4_E_Ow3gB7AY16rGi-g5LSr"

//#define BLYNK_TEMPLATE_ID "TMPLxWgIj1Nn"
//#define BLYNK_DEVICE_NAME "Heart Rate Monitor"
//#define BLYNK_AUTH_TOKEN "9XTFrUr9ROuIcHTGjL3q2AYS8BkemEOw"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>



#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;

//char ssid[] = "ZTE_2.4G_6a67KT";  // type your wifi name
//char pass[] = "Siji2345";

char ssid[] = "ASUS_X00TD";  // type your wifi name
char pass[] = "iqbal123";

BlynkTimer timer;
#define SCREEN_WIDTH 128 //--> Lebar OLED Display
#define SCREEN_HEIGHT 64 //--> Tinggi OLED Display
#define OLED_RESET     -1 // Reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long previousMillisGetHR = 0; //--> menyimpan waktu millis ketika sudah diperbaharui (heartbeat)
unsigned long previousMillisResultHR = 0; //--> menyimpan waktu millis ketika sudah diperbaharui (BPM)
const long intervalGetHR = 20; //--> Interval pebacaan.
const long intervalResultHR = 10000; //--> Interval pembacaan setiap 10 detik
int PulseSensorSignal; 
const int PulseSensorHRWire = A0; 
int UpperThreshold = 540; 
int LowerThreshold = 520; 
int cntHB = 0; 
boolean ThresholdStat = true; //--> variabel untuk mentriger detak jantung
int BPMval = 0; 

int x=0; 
int y=0; 
int lastx=0;
int lasty=0;

//----------------------------------------'HardyIndustriesLogo', 128x64px
const unsigned char HardyIndustriesLogo [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x9f, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x83, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0xc0, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
  0xfc, 0xf8, 0xf8, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x60, 0x9e, 0x3f, 0xfe, 0x38, 0x7f, 0x0f, 0xf0, 0x7f, 0xc0, 0x3f, 0x80, 0x7e, 0x0f, 0xc1, 
  0xf0, 0x00, 0x87, 0x1f, 0xfe, 0x78, 0xff, 0x0f, 0xf0, 0x7f, 0x80, 0x1f, 0x00, 0x0e, 0x0f, 0x83, 
  0xf0, 0x00, 0x83, 0x8f, 0xfc, 0x70, 0xff, 0x0f, 0xe0, 0x7f, 0x80, 0x0f, 0x00, 0x07, 0x0f, 0x07, 
  0xf8, 0x00, 0xf0, 0xc7, 0xfc, 0x70, 0xfe, 0x1f, 0xc0, 0x7f, 0x0f, 0x0f, 0x0f, 0x03, 0x06, 0x0f, 
  0xf8, 0x07, 0x78, 0xe7, 0xfc, 0xf0, 0xfe, 0x1f, 0x80, 0x7f, 0x0f, 0x0e, 0x1f, 0x83, 0x84, 0x1f, 
  0xf8, 0x18, 0x0c, 0x63, 0xfc, 0xe1, 0xfe, 0x1f, 0x84, 0x7f, 0x0f, 0x0e, 0x1f, 0xc3, 0x80, 0x3f, 
  0xf8, 0x20, 0x06, 0x33, 0xfc, 0xe1, 0xfc, 0x3f, 0x0c, 0x3e, 0x1e, 0x1e, 0x1f, 0xc3, 0x80, 0x7f, 
  0xf8, 0x20, 0x03, 0x33, 0xf9, 0xe0, 0x00, 0x3e, 0x1c, 0x3e, 0x00, 0x3c, 0x3f, 0xc3, 0xc1, 0xff, 
  0xe0, 0x43, 0xc3, 0x31, 0xf9, 0xc0, 0x00, 0x3c, 0x3c, 0x3e, 0x00, 0x7c, 0x3f, 0xc3, 0xc3, 0xff, 
  0x80, 0x47, 0xe1, 0x31, 0xf9, 0xc0, 0x00, 0x7c, 0x3c, 0x3e, 0x00, 0x7c, 0x3f, 0x87, 0xc3, 0xff, 
  0x80, 0x47, 0xe1, 0xff, 0xfb, 0xc3, 0xf8, 0x78, 0x00, 0x3c, 0x38, 0x78, 0x7f, 0x07, 0xc3, 0xff, 
  0x80, 0x47, 0xe1, 0xf1, 0xfb, 0x87, 0xf8, 0x70, 0x00, 0x3c, 0x38, 0x78, 0x7e, 0x0f, 0x87, 0xff, 
  0x80, 0x47, 0xe0, 0x06, 0xf3, 0x87, 0xf8, 0xf0, 0x00, 0x3c, 0x78, 0x38, 0x7c, 0x1f, 0x87, 0xff, 
  0x80, 0x43, 0xc3, 0xf0, 0xf7, 0x87, 0xf0, 0xe1, 0xfc, 0x38, 0x78, 0x38, 0x00, 0x3f, 0x87, 0xff, 
  0xf8, 0x20, 0x03, 0xff, 0xf7, 0x0f, 0xf0, 0xc3, 0xfc, 0x18, 0x7c, 0x30, 0x00, 0xff, 0x0f, 0xff, 
  0xf8, 0x20, 0x03, 0x87, 0xf7, 0x0f, 0xf0, 0x83, 0xfc, 0x18, 0xfc, 0x30, 0x03, 0xff, 0x0f, 0xff, 
  0xf8, 0x10, 0x00, 0x37, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x06, 0x27, 0x87, 0xef, 0x11, 0xe2, 0x07, 0x86, 0x18, 0x00, 0x00, 0x70, 0x80, 0xc7, 0xff, 
  0xf8, 0x00, 0x27, 0xff, 0xee, 0x00, 0xc2, 0x01, 0x8e, 0x00, 0x00, 0x00, 0x10, 0x01, 0x03, 0xff, 
  0xf0, 0x00, 0x23, 0xf1, 0xee, 0x20, 0xc2, 0x01, 0x0e, 0x20, 0x00, 0x02, 0x11, 0x00, 0x13, 0xff, 
  0xf0, 0x00, 0x30, 0x05, 0xfc, 0x20, 0x44, 0x30, 0x0c, 0x03, 0xe1, 0xc2, 0x20, 0x1e, 0x1f, 0xff, 
  0xf8, 0x60, 0x98, 0x05, 0xdc, 0x00, 0x04, 0x30, 0x1c, 0x20, 0xe1, 0xc0, 0x60, 0x03, 0x07, 0xff, 
  0xfc, 0xf8, 0xce, 0x73, 0xdc, 0x40, 0x04, 0x61, 0x18, 0x70, 0xe3, 0x84, 0x62, 0x1f, 0x83, 0xff, 
  0xff, 0xf8, 0xe0, 0xbf, 0xf8, 0x46, 0x08, 0x03, 0x00, 0x00, 0x43, 0x84, 0x00, 0x1f, 0x07, 0xff, 
  0xff, 0xf8, 0xf0, 0xbf, 0xf8, 0x06, 0x08, 0x07, 0x00, 0x80, 0xc3, 0x0c, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xf8, 0xfe, 0x7f, 0xf8, 0x07, 0x08, 0x1f, 0x83, 0x83, 0xc7, 0x0c, 0x04, 0x00, 0x1f, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
//---------------------------------------------------Logo Hati-----------------------------------
const unsigned char Heart_Icon [] PROGMEM = {
  0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
  0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};

void setup() {
  Serial.begin(115200); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //--> Jangan proses, loop selamanya
  }  
  //----------------------------------------menampilkan logo
  display.clearDisplay(); 
  display.drawBitmap(0, 0, HardyIndustriesLogo, 128, 64, WHITE);
  display.display();
  delay(3000); 
  //----------------------------------------menampilkan nilai bpm
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 12);
  display.print("     Please wait");
  
  display.setCursor(0, 22);
  display.print("     10  seconds");
  
  display.setCursor(0, 32); 
  display.print("       to get");
  
  display.setCursor(0, 42);
  display.print(" the Heart Rate value");
  
  display.display(); 
  delay(3000);
  
  //----------------------------------------tampilan pertama mebuat nilai bpm 0
  display.clearDisplay(); 
  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
  
  display.drawLine(0, 43, 127, 43, WHITE);
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 48); 
  display.print(": 0 BPM");
  display.display(); 
  Serial.println();
  Serial.println("Please wait 10 seconds to get the BPM Value");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Blynk.config(auth);

}

void loop() {
  Blynk.run();
  GetHeartRate(); 
} 

void GetHeartRate() {
  //----------------------------------------proses pembacaan.
  unsigned long currentMillisGetHR = millis();

  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;

    PulseSensorSignal = analogRead(PulseSensorHRWire);
    Serial.print(PulseSensorSignal);
    Serial.print("   ");

    if (PulseSensorSignal > UpperThreshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
    }

    if (PulseSensorSignal < LowerThreshold) {
      ThresholdStat = true;
    }
    
    DrawGraph();
  }
  //----------------------------------------proses meubah dari pembacaan ke nilai bpm
  unsigned long currentMillisResultHR = millis();

  if (currentMillisResultHR - previousMillisResultHR >= intervalResultHR) {
    previousMillisResultHR = currentMillisResultHR;

    BPMval = cntHB * 6;
    Serial.print("BPM : ");
    Serial.println(BPMval);
    
    display.fillRect(20, 48, 108, 18, BLACK);
    
    display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE); 
    display.drawLine(0, 43, 127, 43, WHITE); 
  
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 48);
    display.print(": ");
    display.print(BPMval);
    display.print(" BPM");
    display.display(); 
    Blynk.virtualWrite(V0, BPMval);
    cntHB = 0;
  }
}
//--------------------------------------------------------------------------------membuat grafik
void DrawGraph() {
  if (x > 127) {
    display.fillRect(0, 0, 128, 42, BLACK);
    x = 0;
    lastx = 0;
  }
  int ySignal = PulseSensorSignal; 
  
  if (ySignal > 850) ySignal = 850;
  if (ySignal < 350) ySignal = 350;

  int ySignalMap = map(ySignal, 350, 850, 0, 40);    
  
  y = 40 - ySignalMap;
  Serial.println(y);
  display.writeLine(lastx,lasty,x,y,WHITE);
  display.display();   
  lastx = x;
  lasty = y;
  
  x++;
}