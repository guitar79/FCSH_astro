/* How to use the DHT-22 sensor with Arduino nano
Temperature and humidity sensor
*/

//Libraries
#include <DHT.h>;

//Constants
#define DHTPIN 3 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum; //Stores humidity value
float temp; //Stores temperature value

int LED;

void setup()
{
Serial.begin(9600);
dht.begin();
LED = 2;
pinMode(LED, OUTPUT);
}

void loop()
{

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  
delay(2000);
//Read data and store it to variables hum and temp
hum = dht.readHumidity();
temp = dht.readTemperature();
//Print temp and humidity values to serial monitor
Serial.print("Humidity: ");
Serial.print(hum);
Serial.print(" %, Temp: ");
Serial.print(temp);
Serial.println(" Celsius");
delay(2000); //Delay 2 sec.
}
