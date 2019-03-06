#include <Ethernet.h>
#include <SPI.h>
#include "DHT.h"

//Configuracion pin DHT
DHT dht(7, DHT11);

//Configuración pines Ultrasonido
const unsigned int TRIG_PIN=8;
const unsigned int ECHO_PIN=9;
const unsigned int BAUD_RATE=9600;

//Referencias de comparación
const unsigned int RefT=25;
const unsigned int RefNivel=5;
const unsigned int RefH=80;

//Configuracion Actuadores
const unsigned int VENTI=2;    //Ventilador
const unsigned int REFT=27;    //Referencia Temperatura
const unsigned int AGUA=3;    //Agua
const unsigned int REFD=20;    //Referencia NIVEL
const unsigned int REFHT=650;    //Referencia Humedad Tierra
unsigned int HT=27;    //Humedad Tierra

//Configuración RED
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 }; // RESERVED MAC ADDRESS
EthernetClient client;

String data;

void setup() { 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(VENTI, OUTPUT);
  pinMode(AGUA, OUTPUT);
  
  Serial.begin(BAUD_RATE);

  // PREPARAR LA LIBRERIA PARA COMUNICARSE CON EL SENSOR
  dht.begin();

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP"); 
	}
 
	data = "";
}

void loop(){
HT = (int)analogRead(0);
delay(150000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
//delay(5000);

  //Lectura de Datos
  // LEER LA HUMEDAD USANDO EL METRODO READHUMIDITY
  float h = dht.readHumidity();
  // LEER LA TEMPERATURA USANDO EL METRODO READTEMPERATURE
  float t = dht.readTemperature();
  // REVISAR QUE LOS RESULTADOS SEAN VALORES NUMERICOS VALIDOS, INDICANDO QUE LA COMUNICACION ES CORRECTA
  if (isnan(h) || isnan(t)) {
    Serial.println("Falla al leer el sensor DHT11!");
    return;
  }
  //Leer Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
  int distance= duration/29/2;

//REVISION DE NIVELES
  if(t>REFT)
  {
      digitalWrite(VENTI, HIGH);   
  }
  else
  {
      digitalWrite(VENTI, LOW);
  }

  
  if(distance<REFD)
  {
    digitalWrite(4, LOW);   
      
      if(HT<REFHT)
      {
          digitalWrite(AGUA, HIGH);     
          delay(5000);
          digitalWrite(AGUA, LOW);     
      }  
  }
  else
  {   
      digitalWrite(4, HIGH);   
      
  }
  
    if(distance>REFD)
    {
        digitalWrite(AGUA, HIGH);   
    }else{
        digitalWrite(AGUA, LOW);   
    }

  data = "T=" + (String)t + "&H=" + (String)h + "&d=" + (String)distance+"&HT=" + (String)HT;

	if (client.connect("proyectoiieagricola.000webhostapp.com",80)) { // REPLACE WITH YOUR SERVER ADDRESS
		client.println("POST /Users.php HTTP/1.1"); 
		client.println("Host: proyectoiieagricola.000webhostapp.com"); // SERVER ADDRESS HERE TOO
		client.println("Content-Type: application/x-www-form-urlencoded"); 
		client.print("Content-Length: "); 
		client.println(data.length()); 
		client.println(); 
		client.print(data); 
    Serial.println(data); 
    Serial.println("Enviado"); 
	} 

	if (client.connected()) { 
		client.stop();	// DISCONNECT FROM THE SERVER
	}

}
