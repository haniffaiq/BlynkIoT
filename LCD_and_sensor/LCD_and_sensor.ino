#define BLYNK_TEMPLATE_ID "TMPLjXwjEeXW"
#define BLYNK_DEVICE_NAME "Tugas Akhir"
#define BLYNK_AUTH_TOKEN "rsHReQUIa4_E_Ow3gB7AY16rGi-g5LSr"

//#define BLYNK_TEMPLATE_ID "TMPLxWgIj1Nn"
//#define BLYNK_DEVICE_NAME "Heart Rate Monitor"
//#define BLYNK_AUTH_TOKEN "9XTFrUr9ROuIcHTGjL3q2AYS8BkemEOw"


#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
 
// Konstruk object LCD dengan alamat I2C
// Ganti 0x3F sesuai dengan alamat I2C modul kalian
// Jika tidak tahu dapat menggunakan LCD I2C Scanner

// GPIO where the DS18B20 is connected to
#define BLYNK_PRINT Serial
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
//char ssid[] = "ZTE_2.4G_6a67KT";  // type your wifi name
//char pass[] = "Siji2345";

char ssid[] = "ASUS_X00TD";  // type your wifi name
char pass[] = "iqbal123";
BlynkTimer timer;
const int oneWireBus = 14;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  WiFi.begin(ssid, pass);
  lcd.begin(16,2);
  lcd.init();
 
  // Nyalakan backlight
  lcd.backlight();
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
      lcd.setCursor(0, 0);
      lcd.print("Connecting to WiFi..");
  }
  Blynk.config(auth);
  // Pemanggilan pertama memerlukan parameter jumlah kolom dan baris
  // Ini harus sama dengan yang dimasukan pada konstruktor.
 
  // Pindahkan kursor ke kolom 0 dan baris 0
  // (baris 1)
  lcd.setCursor(0, 0);
 
  // Cetak hellow ke layar
  lcd.print("Device connected");
}

void loop() {
  Blynk.run();
  getSuhu();

}

void getSuhu(){
  
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    Blynk.virtualWrite(V1, temperatureC);
    lcd.setCursor(0, 1);
    lcd.print(temperatureC);
    lcd.print(" C");
    delay(1000);
  }
