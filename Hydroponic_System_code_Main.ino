
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Arduino.h>
#include "MQ135.h"
#include <SPI.h>
#include <Wire.h>
#include "Arduino.h"
#include <DS3232RTC.h>
#include <dht11.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "smart-hydroponics-system-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "1nF8bMO1oe30oMiDXpRBx76RxOFYK6uHAUGbFt5W"
#define WIFI_SSID "COD"
#define WIFI_PASSWORD "ww0764473165"
FirebaseData firebaseData;


#define VREF 5.0
#define SCOUNT  30
#define RLOAD 22.0
MQ135 gasSensor = MQ135(A0);
int val;
int sensorValue = 0;

String Auto;
String Lightstatus;

#define S0 D0
#define S1 D3
#define S2 3
#define SIG A0
String Heater;
#define heater D7


int decimal = 2;
int sensor0;
int sensor1;
int sensor2;
int sensor3;
int sensor4;
int sensor5;

float vIn;
float vOut;
float voltageSensorVal;
const float factor = 3.168;
const float vCC = 5.00;
int analogBuffer[SCOUNT];
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 27;
float tdsv;
int TDSVal;
int PHVal;
int waterlevelVal;
int coolenttemperaturemaxVal;
int coolenttemperatureminVal;
int watertemperatureVal;
int maxhumidity;
int airquality;
int fertilzerH;
int fertilzerM;
int fertilzerS;
int heaterS;
float calibration_value = 21.34 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;
#define DHT11PIN D4
dht11 DHT11;
float humidity;
int tempsen;



#define SENSOR_PIN D5
#define SENSOR_PIN1 D6
OneWire oneWire(SENSOR_PIN);
OneWire oneWire1(SENSOR_PIN1);
DallasTemperature tempSensor(&oneWire);
DallasTemperature tempSensor1(&oneWire1);

float tempCelsius;
float tempCelsius1;


DS3232RTC myRTC;

String fulltime;
int h;
int m;
int s;


void setup() {

  Serial.begin(9600);
  Serial.println("Serial communication started\n\n");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.reconnectWiFi(true);
  delay(500);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(SIG, INPUT);

  tempSensor.begin();   //Ds16b
  tempSensor1.begin();
  myRTC.begin();
  setSyncProvider(myRTC.get);

  pinMode(heater, OUTPUT);
  digitalWrite(heater, HIGH);
}

void loop() {



  Firebase.getString(firebaseData, "/Data/Auto"); {
    if (firebaseData.dataType() == "string") {
      Auto = firebaseData.stringData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/TDS"); {
    if (firebaseData.dataType() == "int") {
      TDSVal = firebaseData.intData();

    }
  }
  //delay(200);
  Firebase.getInt(firebaseData, "/Setting/PH"); {
    if (firebaseData.dataType() == "int") {
      PHVal = firebaseData.intData();

    }
  }
  //delay(200);
  Firebase.getInt(firebaseData, "/Setting/Water_Level_Max"); {
    if (firebaseData.dataType() == "int") {
      waterlevelVal = firebaseData.intData();

    }
  }
  //delay(200);
  Firebase.getInt(firebaseData, "/Setting/Coolent_Temperature_Max"); {
    if (firebaseData.dataType() == "int") {
      coolenttemperaturemaxVal = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Coolent_Temperature_Min"); {
    if (firebaseData.dataType() == "int") {
      coolenttemperatureminVal = firebaseData.intData();

    }
  }
  //delay(200);
  Firebase.getInt(firebaseData, "/Setting/Water_Temperature_Max"); {
    if (firebaseData.dataType() == "int") {
      watertemperatureVal = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Max_Humidity"); {
    if (firebaseData.dataType() == "int") {
      maxhumidity = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Max_Air_Quality"); {
    if (firebaseData.dataType() == "int") {
      airquality = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Fertilizing_Time_H"); {
    if (firebaseData.dataType() == "int") {
      fertilzerH = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Fertilizing_Time_M"); {
    if (firebaseData.dataType() == "int") {
      fertilzerM = firebaseData.intData();

    }
  }
  Firebase.getInt(firebaseData, "/Setting/Fertilizing_Time_S"); {
    if (firebaseData.dataType() == "int") {
      fertilzerS = firebaseData.intData();

    }
  }

    Firebase.getInt(firebaseData, "/Setting/Heater_Start"); {
    if (firebaseData.dataType() == "int") {
      heaterS = firebaseData.intData();

    }
  }


  //delay(200);
  Firebase.getString(firebaseData, "/Controllers/Heater"); {
    if (firebaseData.dataType() == "string") {
      Heater = firebaseData.stringData();

    }
  }


  if (Auto == "1") {
    Light_Time();
    Channel();
    main_temp();

    
  if (tempsen > heaterS) {
    digitalWrite(heater, LOW);
    Firebase.setString(firebaseData, "/Controllers/Heater", "1");
  } else if  (tempsen < heaterS) {
    digitalWrite(heater, HIGH);
    Firebase.setString(firebaseData, "/Controllers/Heater", "0");
  }

    
    //////////////AirQuality////////////////////////
    if (sensor0  > airquality) {
      Firebase.setString(firebaseData, "/Controllers/Fan", "1");
      Firebase.setString(firebaseData, "/Infromation/Air_Quality", "Bad");
    }
    else {
      Firebase.setString(firebaseData, "/Controllers/Fan", "0");
      Firebase.setString(firebaseData, "/Infromation/Air_Quality", "Good");
    }

    /////////////Voltage///////////////////////////

    if (vIn < 4.31) {
    }

    ///////////////TDS & PH level///////////////////////////

    if (tdsv < TDSVal) {
 

      if ((h == fertilzerH) && (m == fertilzerM)) {
        Firebase.setString(firebaseData, "/Controllers/Fertilizer_Pump_1", "1");
        Firebase.setString(firebaseData, "/Controllers/Oxygen_Pump", "1");  
      }

      else {
        Firebase.setString(firebaseData, "/Controllers/Fertilizer_Pump_1", "0");
        Firebase.setString(firebaseData, "/Controllers/Oxygen_Pump", "0");

      }
    }

    else {
      Firebase.setString(firebaseData, "/Controllers/Fertilizer_Pump_1", "0");
      Firebase.setString(firebaseData, "/Controllers/Oxygen_Pump", "0");

    }


    ////////////Water Level///////////////////////////
    if (sensor4 > waterlevelVal) {

      Firebase.setString(firebaseData, "/Controllers/Water_Pump", "0");
    } else {
      Firebase.setString(firebaseData, "/Controllers/Water_Pump", "1");

    }
    ////////////current///////////////////////////
    if (sensor5 > 2) {
      Firebase.setString(firebaseData, "/Infromation/System_Health", "Good");
    } else {
      Firebase.setString(firebaseData, "/Infromation/System_Health", "Bad");
    }

    ////////////Humidity///////////////////////////
    if (humidity < maxhumidity) {
      Firebase.setString(firebaseData, "/Controllers/Humidifier", "1");

    } else {
      Firebase.setString(firebaseData, "/Controllers/Humidifier", "0");

    }

    ////////////Water Temperature///////////////////////////
    if (tempCelsius < watertemperatureVal) {
      Firebase.setString(firebaseData, "/Infromation/Water_Temperature", "Good");
    } else {
      Firebase.setString(firebaseData, "/Infromation/Water_Temperature", "Bad");
    }

    ////////////Coolent Temperature///////////////////////////
    if (tempCelsius1 > coolenttemperaturemaxVal) {

      Firebase.setString(firebaseData, "/Controllers/Cooler", "0");
      Firebase.setString(firebaseData, "/Infromation/Coolent_Temperature", "Bad");
    } else if (tempCelsius1 < coolenttemperatureminVal) {

      Firebase.setString(firebaseData, "/Controllers/Cooler", "1");
      Firebase.setString(firebaseData, "/Infromation/Coolent_Temperature", "Good");
    }

  } else {
    Channel();
  }
}


void Channel() {

  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, LOW);
  sensor0 = analogRead(SIG);
  val = analogRead(SIG);
  Serial.print("Sensor 1 + AirQuality=");
  Serial.print(val);  // prints the value read
  Serial.println(" PPM");
  Firebase.setString(firebaseData, "/Data/AirQuality", val);
  //delay(1000);
  delay(100);



  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, LOW);
  sensor1 = analogRead(SIG);
  Serial.print("Sensor 2 + Voltage = ");
  Serial.print(vIn);
  Serial.println("V");
  voltageSensorVal = analogRead(SIG);    // read the current sensor value (0 - 1023)
  vOut = (voltageSensorVal / 1024) * vCC;             // convert the value to the real voltage on the analog pin
  vIn =  vOut * factor;                               // convert the voltage on the source by multiplying with the factor
  Firebase.setString(firebaseData, "/Data/Voltage", vIn);
  //delay(1000);
  delay(100);



  digitalWrite(S0, LOW); digitalWrite(S1, HIGH); digitalWrite(S2, LOW);
  sensor2 = analogRead(SIG);
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U)  //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(SIG);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
    tdsv = tdsValue;


    Serial.print("Sensor 3 + voltage:");
    Serial.print(averageVoltage, 2);
    Serial.print("V   ");
    Serial.print("TDS Value:");
    Serial.print(tdsv, 0);
    Serial.println("ppm");
    Firebase.setString(firebaseData, "/Data/TDS Value", tdsv);

  }

  delay(100);

  //delay(1000);







  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH); digitalWrite(S2, LOW);
  sensor3 = analogRead(SIG);
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(SIG);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 3 / 1024 / 6;
  ph_act = -5.70 * volt + calibration_value;;
  Serial.print("Sensor 4 + PH level: ");
  Serial.println(ph_act);
  Firebase.setString(firebaseData, "/Data/PH level", ph_act);

  delay(100);



  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, HIGH);
  sensor4 = analogRead(SIG);
  int value = analogRead(SIG);
  Serial.print("Sensor 5 + Water Level : ");
  Serial.print(value);
  Firebase.setString(firebaseData, "/Data/Water Level Value", value);
  if (value <= 100) {
    Serial.println("  Empty ");
    Firebase.setString(firebaseData, "/Data/Water Level", "Empty");
  } else if (value > 100 && value < 300) {
    Serial.println("  LOW   ");
    Firebase.setString(firebaseData, "/Data/Water Level", "LOW");
  } else if (value > 300 && value < 330) {
    Serial.println("  Medium");
    Firebase.setString(firebaseData, "/Data/Water Level", "Medium");
  } else if (value > 330) {
    Serial.println("  HIGH  ");
    Firebase.setString(firebaseData, "/Data/Water Level", "HIGH");
  }


  delay(100);


  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, HIGH);
  sensor5 = analogRead(SIG);
  int adc = analogRead(SIG);
  float voltage = adc * 5 / 1023.0;
  float current = (voltage - 2.5) / 0.185;
  if (current < 0.16) {
    current = 0;

  }
  float realcurrnt = current - 7;
  Serial.print("Sensor 6 + current : "); Serial.println(realcurrnt);
  Firebase.setFloat(firebaseData, "/Data/Current Usage", realcurrnt);
  delay(100);

}

void main_temp()
{


  int chk = DHT11.read(DHT11PIN);
  tempSensor.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius
  tempSensor1.requestTemperatures();
  tempCelsius1 = tempSensor1.getTempCByIndex(0);

  Serial.print("Sensor 7 + Humidity (%): ");
  Serial.print((float)DHT11.humidity , 2);
  Serial.print(" ");
  humidity = DHT11.humidity;
  tempsen = DHT11.temperature;
  Firebase.setString(firebaseData, "/Data/Area humidity", humidity);

  Serial.print("Temperature   (C): ");
  Serial.println((float)DHT11.temperature, 2);
 // Serial.print(tempsen);
  Firebase.setString(firebaseData, "/Data/Area Temperature", DHT11.temperature);

  Serial.print("Sensor 8 + Water Temperature: ");
  Serial.print(tempCelsius);    // print the temperature in Celsius
  Serial.println("°C");
  Firebase.setFloat(firebaseData, "/Data/Water Temperature", tempCelsius);

  Serial.print("Sensor 9 + Coolent Temperature: ");
  Serial.print(tempCelsius1);    // print the temperature in Celsius
  Serial.println("°C");
  Firebase.setFloat(firebaseData, "/Data/Coolent Temperature", tempCelsius1);


}



void Getday() {
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());


  h = hour();
  m = minute();
  s = second();

}

void Light_Time() {


  Getday();

  if ((h == 5) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "1");
  }
  if ((h == 6) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "2");
  }
  if ((h == 6) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "3");
  }
  if ((h == 7) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "4");
  }
  if ((h == 7) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "5");
  }
  if ((h == 8) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "6");
  }
  if ((h == 8) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "7");
  }
  if ((h == 9) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "8");
  }
  if ((h == 9) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "9");
  }
  if ((h == 10) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "10");
  }
  if ((h == 10) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "11");
  }
  if ((h == 11) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "12");
  }
  if ((h == 11) && (m >= 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "13");
  }
  if (h == 12) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "14");
  }
  if ((h == 13) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "15");
  }
  if ((h == 13) && (m > 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "16");
  }
  if ((h == 14) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "17");
  }
  if ((h == 14) && (m > 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "18");
  }
  if ((h == 15) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "19");
  }
  if ((h == 15) && (m > 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "20");
  }
  if ((h == 16) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "21");
  }
  if ((h == 16) && (m > 30) && ( m <= 59)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "22");
  }
  if ((h == 17) && (m >= 0) && ( m <= 30)) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "23");
  }
  if (((h >= 18) && (h <= 23)) || (h == 00) || ((h >= 1) && (h <= 5) && (m >= 0) && ( m <= 30))) {
    Firebase.setString(firebaseData, "/Controllers/Light_Status", "24");
  }

}




int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
