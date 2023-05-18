#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Arduino.h>


#define FIREBASE_HOST "smart-hydroponics-system-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "1nF8bMO1oe30oMiDXpRBx76RxOFYK6uHAUGbFt5W"
#define WIFI_SSID "COD"
#define WIFI_PASSWORD "ww0764473165"

FirebaseData firebaseData;


String Cooler;
String Fan;
String Fertilizer_Pump_1;
String Fertilizer_Pump_2;
String Heater;
String Humidifier;
String Oxygen_Pump;
String Water_Pump;
String light_status;

#define Gate D0
#define water_Pump D1
#define cooler D2
#define fan D3
//#define heater D4
#define humidifier D5
#define oxygen_Pump D6
#define fertilizer_Pump_1 D7
#define fertilizer_Pump_2 D4

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

  delay(2000);

  pinMode(cooler, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(fertilizer_Pump_1, OUTPUT);
  pinMode(fertilizer_Pump_2, OUTPUT);
  //pinMode(heater, OUTPUT);
  pinMode(humidifier, OUTPUT);
  pinMode(oxygen_Pump, OUTPUT);
  pinMode(water_Pump, OUTPUT);

  pinMode(Gate, OUTPUT);



  digitalWrite(cooler, HIGH);
  digitalWrite(fan, HIGH);
  digitalWrite(fertilizer_Pump_1, HIGH);
  digitalWrite(fertilizer_Pump_2, HIGH);
  //digitalWrite(heater, HIGH);
  digitalWrite(humidifier, HIGH);
  digitalWrite(oxygen_Pump, HIGH);
  digitalWrite(water_Pump, HIGH);

}

void loop() {

  firebase();
  Brightness();
  digitalWrite(fertilizer_Pump_1, HIGH);
  if (Cooler == "1") {
    digitalWrite(cooler, LOW);
    Serial.println("Cooler ON");
  } else if (Cooler == "0") {
    digitalWrite(cooler, HIGH);
    Serial.println("Cooler OFF");
  }

  if (Fan == "1") {
    digitalWrite(fan, LOW);
    Serial.println("Fan ON");
  } else if (Fan == "0") {
    digitalWrite(fan, HIGH);
    Serial.println("Fan OFF");
  }

  if (Fertilizer_Pump_1 == "1") {
    digitalWrite(fertilizer_Pump_1, LOW);
    Serial.println("Fertilizer_Pump_1 ON");
  } else if (Fertilizer_Pump_1 == "0") {
    digitalWrite(fertilizer_Pump_1, HIGH);
    Serial.println("Fertilizer_Pump_1 OFF");
  }

  if (Fertilizer_Pump_2 == "1") {
    digitalWrite(fertilizer_Pump_2, LOW);
    Serial.println("Fertilizer_Pump_2 ON");
  } else if (Fertilizer_Pump_2 == "0") {
    digitalWrite(fertilizer_Pump_2, HIGH);
    Serial.println("Fertilizer_Pump_2 OFF");
  }

  if (Humidifier == "1") {
    digitalWrite(humidifier, LOW);
    Serial.println("Humidifier ON");
  } else if (Humidifier == "0") {
    digitalWrite(humidifier, HIGH);
    Serial.println("Humidifier OFF");
  }

  if (Oxygen_Pump == "1") {
    digitalWrite(oxygen_Pump, LOW);
    Serial.println("Oxygen_Pump ON");
  } else if (Oxygen_Pump == "0") {
    digitalWrite(oxygen_Pump, HIGH);
    Serial.println("Oxygen_Pump OFF");
  }

  if (Water_Pump == "1") {
    digitalWrite(water_Pump, LOW);
    Serial.println("Water_Pump ON");
  } else if (Water_Pump == "0") {
    digitalWrite(water_Pump, HIGH);
    Serial.println("Water_Pump OFF");
  }

}

void Brightness() {

  if (light_status == "1") {
    analogWrite(Gate, 18.2);
  }
  if (light_status == "2") {
    analogWrite(Gate, 36.4);
  }
  if (light_status == "3") {
    analogWrite(Gate, 54.6);
  }
  if (light_status == "4") {
    analogWrite(Gate, 72.8);
  }
  if (light_status == "5") {
    analogWrite(Gate, 91);
  }
  if (light_status == "6") {
    analogWrite(Gate, 109.2);
  }
  if (light_status == "7") {
    analogWrite(Gate, 127.4);
  }
  if (light_status == "8") {
    analogWrite(Gate, 145.6);
  }
  if (light_status == "9") {
    analogWrite(Gate, 163.8);
  }
  if (light_status == "10") {
    analogWrite(Gate, 182);
  }
  if (light_status == "11") {
    analogWrite(Gate, 200);
  }
  if (light_status == "12") {
    analogWrite(Gate, 218.4);
  }
  if (light_status == "13") {
    analogWrite(Gate, 236.6);
  }
  if (light_status == "14") {
    analogWrite(Gate, 255);
  }
  if (light_status == "15") {
    analogWrite(Gate, 231.8);
  }
  if (light_status == "162") {
    analogWrite(Gate, 207.9);
  }
  if (light_status == "17") {
    analogWrite(Gate, 184.8);
  }
  if (light_status == "18") {
    analogWrite(Gate, 161.7);
  }
  if (light_status == "19") {
    analogWrite(Gate, 138.6);
  }
  if (light_status == "20") {
    analogWrite(Gate, 115.5);
  }
  if (light_status == "21") {
    analogWrite(Gate, 92.4);
  }
  if (light_status == "22") {
    analogWrite(Gate, 69.3);
  }
  if (light_status == "23") {
    analogWrite(Gate, 46.2);
  }
  if (light_status == "24") {
    analogWrite(Gate, 0);
  }
}


void firebase() {

  Firebase.getString(firebaseData, "/Controllers/Light_Status");

  if (firebaseData.dataType() == "string") {

    light_status = firebaseData.stringData();

  }


  Firebase.getString(firebaseData, "/Controllers/Cooler");

  if (firebaseData.dataType() == "string") {

    Cooler = firebaseData.stringData();
    // Serial.println(Cooler);
  }
  Firebase.getString(firebaseData, "/Controllers/Fan");

  if (firebaseData.dataType() == "string") {

    Fan = firebaseData.stringData();
    //Serial.println(Fan);
  }
  Firebase.getString(firebaseData, "/Controllers/Fertilizer_Pump_1");

  if (firebaseData.dataType() == "string") {

    Fertilizer_Pump_1 = firebaseData.stringData();
    // Serial.println(Fertilizer_Pump_1);
  }
  Firebase.getString(firebaseData, "/Controllers/Fertilizer_Pump_2");

  if (firebaseData.dataType() == "string") {

    Fertilizer_Pump_2 = firebaseData.stringData();
    // Serial.println(Fertilizer_Pump_2);
  }
  Firebase.getString(firebaseData, "/Controllers/Heater");

  if (firebaseData.dataType() == "string") {

    Heater = firebaseData.stringData();
    // Serial.println(Heater);
  }
  Firebase.getString(firebaseData, "/Controllers/Humidifier");

  if (firebaseData.dataType() == "string") {

    Humidifier = firebaseData.stringData();
    // Serial.println(Humidifier);
  }
  Firebase.getString(firebaseData, "/Controllers/Oxygen_Pump");

  if (firebaseData.dataType() == "string") {

    Oxygen_Pump = firebaseData.stringData();
    // Serial.println(Oxygen_Pump);
  }
  Firebase.getString(firebaseData, "/Controllers/Water_Pump");

  if (firebaseData.dataType() == "string") {

    Water_Pump = firebaseData.stringData();
    // Serial.println(Water_Pump);
  }


}
